%module(directors="1") formast
%{
#include "formast.hpp"

#ifdef SWIGPYTHON
// extra wrapper function to map FILE * to istream, as Python can't do istream
// TODO: this is a gcc extension, need to find out how to do this on msvc
#include <ext/stdio_filebuf.h>
bool parse_xml(FILE * f, formast::Expr & e) {
    __gnu_cxx::stdio_filebuf<char> ibuf(f, std::ios_base::in);
    std::istream is(&ibuf);
    formast::parse_xml(is, e);
}
#endif
%}

// tell swig about boost::uint64_t
namespace boost {
    typedef unsigned long long uint64_t;
}

namespace formast {
    // Expr is a typedef in formast.hpp
    // swig will not wrap it unless it has an implementation
    // whence an empty implementation {}
    // so swig will expose the name, but nothing else
    class Expr {};
}

#ifdef SWIGPYTHON
%ignore formast::parse_xml;
%include "file.i";
bool parse_xml(FILE * f, formast::Expr & e);
#endif

%feature("director") formast::Visitor;
%include "formast.hpp"
