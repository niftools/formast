#ifndef FORMAST_PRINTER_HPP_INCLUDED
#define FORMAST_PRINTER_HPP_INCLUDED

#include <boost/lexical_cast.hpp>
#include <ostream>
#include "formast.hpp"

namespace formast
{

class Printer : public Visitor
{
public:
    Printer(std::ostream & os) : Visitor(), os(os), level(0) {};
    void expr_uint(boost::uint64_t const & n) {
        open_tag("uint", true);
        os << boost::lexical_cast<std::string>(n);
        close_tag("uint", true);
    }
    void expr_id(std::string const & i) {
        open_tag("id", true);
        os << i;
        close_tag("id", true);
    }

    void expr_pos(Expr const & right) {
        open_tag("pos", false);
        expr(right);
        close_tag("pos", false);
    }

    void expr_neg(Expr const & right) {
        open_tag("neg", false);
        expr(right);
        close_tag("neg", false);
    }
    void expr_add(Expr const & left, Expr const & right) {
        open_tag("add", false);
        expr(left);
        expr(right);
        close_tag("add", false);
    }
    void expr_sub(Expr const & left, Expr const & right) {
        open_tag("sub", false);
        expr(left);
        expr(right);
        close_tag("sub", false);
    }
    void expr_mul(Expr const & left, Expr const & right) {
        open_tag("mul", false);
        expr(left);
        expr(right);
        close_tag("mul", false);
    }
    void expr_div(Expr const & left, Expr const & right) {
        open_tag("div", false);
        expr(left);
        expr(right);
        close_tag("div", false);
    }
    virtual void open_tag(std::string const & tag, bool final) {
        indent();
        os << "<" << tag << ">";
        if (!final) {
            os << std::endl;
            level++;
        }
    }
    virtual void close_tag(std::string const & tag, bool final) {
        if (!final) {
            level--;
            indent();
        }
        os << "</" << tag << ">" << std::endl;
    }
    virtual void indent() {
        for (int i = 0; i < level; i++) {
            os << " ";
        };
    }
    std::ostream & os;
    unsigned int level;
};

}

#endif
