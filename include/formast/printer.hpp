#ifndef FORMAST_PRINTER_HPP_INCLUDED
#define FORMAST_PRINTER_HPP_INCLUDED

#include <ostream>
#include "formast.hpp"

namespace formast
{

class Printer : public Visitor
{
public:
    Printer(std::ostream & os);
    void expr_uint(boost::uint64_t const & n);
    void expr_id(std::string const & i);
    void expr_pos(Expr const & right);
    void expr_neg(Expr const & right);
    void expr_add(Expr const & left, Expr const & right);
    void expr_sub(Expr const & left, Expr const & right);
    void expr_mul(Expr const & left, Expr const & right);
    void expr_div(Expr const & left, Expr const & right);
    virtual void open_tag(std::string const & tag, bool final);
    virtual void close_tag(std::string const & tag, bool final);
    virtual void indent();
    std::ostream & os;
    unsigned int level;
};

}

#endif
