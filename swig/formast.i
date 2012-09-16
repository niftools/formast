%module(directors="1") formast
%{
#include <boost/spirit/include/support_istream_iterator.hpp>
#include <fstream>
#include <iostream>

#include "formast.hpp"

typedef formast::Expr Expr;

bool parse_xml(char const * filename, Expr & e)
{
    std::ifstream in(filename, std::ios_base::in);

    if (!in) {
        std::cerr << "Error: Could not open input file: "
                  << filename << std::endl;
        return 1;
    }

    // disable white space skipping
    in.unsetf(std::ios::skipws);

    typedef boost::spirit::istream_iterator Iterator;
    Iterator iter(in);
    Iterator end;
    return formast::parse_xml(iter, end, e);
}

class Visitor : public formast::Visitor<void>
{
public:
    void uint(boost::uint64_t const & n) {};
    void id(std::string const & i) {};
    void pos(Expr const & right) {};
    void neg(Expr const & right) {};
    void add(Expr const & left, Expr const & right) {};
    void sub(Expr const & left, Expr const & right) {};
    void mul(Expr const & left, Expr const & right) {};
    void div(Expr const & left, Expr const & right) {};
};

%}

class Expr {};

bool parse_xml(char const * filename, Expr & e);

%feature("director") Visitor;
class Visitor
{
public:
    Visitor();
    virtual void expr(const Expr & e);
    virtual void uint(boost::uint64_t const & n) ;
    virtual void id(std::string const & i);
    virtual void pos(Expr const & right);
    virtual void neg(Expr const & right);
    virtual void add(Expr const & left, Expr const & right);
    virtual void sub(Expr const & left, Expr const & right);
    virtual void mul(Expr const & left, Expr const & right);
    virtual void div(Expr const & left, Expr const & right);
};
