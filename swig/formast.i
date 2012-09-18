%module(directors="1") formast
%{
#include <boost/spirit/include/support_istream_iterator.hpp>
#include <fstream>
#include <iostream>

#include "formast.hpp"

namespace formast
{

bool parse_xml(char const * filename, Expr & e)
{
    std::ifstream in(filename, std::ios_base::in);

    if (!in) {
        std::cerr << "Error: Could not open input file: "
                  << filename << std::endl;
        return false;
    }

    // disable white space skipping
    in.unsetf(std::ios::skipws);

    typedef boost::spirit::istream_iterator Iterator;
    Iterator iter(in);
    Iterator end;
    return parse_xml(iter, end, e);
}

}

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

    bool parse_xml(char const * filename, Expr & e);
}

%feature("director") formast::Visitor;
%include "formast.hpp"
