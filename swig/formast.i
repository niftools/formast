%module(directors="1") formast
%{
#include "formast.hpp"
%}

// tell swig about boost::uint64_t
namespace boost {
    typedef unsigned long long uint64_t;
}

namespace formast {
    // Expr, Stats, and Top are typedefs in formast.hpp
    // swig will not wrap it unless it has an implementation
    // whence an empty implementation {}
    // so swig will expose the name, but nothing else
    class Expr {};
    class Stats {};
    class Top {};
}

%include "exception.i"
%include "std_string.i"
%ignore formast::Parser::parse_stream;
%ignore formast::XmlParser::parse_stream;
%feature("director") formast::Visitor;
%include "formast.hpp"
