%module(directors="1") formast
%{
#include "formast.hpp"
%}

%include "exception.i"
%include "std_string.i"
%include "std_vector.i"

// tell swig about std::uint64_t
namespace std {
    typedef unsigned long long uint64_t;
}

// tell swig about boost::optional
// implementation note: class extensions need to be defined here
// because the class is a template class, and in this case,
// apparently, swig does not recognise %extend elsewhere
namespace boost {
    template <typename T>
    class optional
    {
    public:
        // ensure get() throws an appropriate exception
        // (C++ implementation uses BOOST_ASSERT which may not be caught)
        %extend {
            T & get() {
                if (self->is_initialized()) {
                    return self->get();
                } else {
                    // SWIG_CATCH_STDEXCEPT turns invalid_argument into SWIG_ValueError
                    throw std::invalid_argument("not initialized");
                }
            };
        }

        // bool conversion: different languages deal with this differently
#ifdef SWIGPYTHON
        %extend {
            // Python 2.x bool conversion
            bool __nonzero__() const {
                return self->is_initialized();
            }
            // Python 3.x bool conversion
            bool __bool__() const {
                return self->is_initialized();
            }
        }
#else
        // default bool conversion
        bool is_initialized() const;
#endif
    };
}

%template(OptionalDoc) boost::optional<formast::Doc>;
%template(OptionalEnumStats) boost::optional<formast::EnumStats>;
%template(OptionalExpr) boost::optional<formast::Expr>;
%template(OptionalStats) boost::optional<formast::Stats>;
%template(OptionalString) boost::optional<std::string>;

%ignore formast::Parser::parse_stream;
%ignore formast::XmlParser::parse_stream;
%feature("director") formast::Visitor;
%include "formast.hpp"
