#ifndef FORMAST_PRINTER_HPP_INCLUDED
#define FORMAST_PRINTER_HPP_INCLUDED

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <ostream>
#include "formast.hpp"

namespace formast
{

class Printer : public Visitor
{
public:
    Printer(std::ostream & os) : Visitor(), os(os), level(0) {};

    void top_class(Class const & class_) {
        open_tag("class", false);
        open_tag("name", true);
        os << class_.name;
        close_tag("name", true);
        if (class_.base_name) {
            open_tag("base_name", true);
            os << class_.base_name.get();
            close_tag("base_name", true);
        }
        if (class_.doc) {
            open_tag("doc", true);
            os << class_.doc.get();
            close_tag("doc", true);
        }
        if (class_.stats) {
            stats(class_.stats.get());
        }
        close_tag("class", false);
    }

    void stats(Stats const & s) {
        open_tag("stats", false);
        Visitor::stats(s);
        close_tag("stats", false);
    }

    void stats_attr(Attr const & attr) {
        open_tag("attr", false);
        open_tag("class_name", true);
        os << attr.class_name;
        close_tag("class_name", true);
        open_tag("name", true);
        os << attr.name;
        close_tag("name", true);
        if (attr.doc) {
            open_tag("doc", true);
            os << attr.doc.get();
            close_tag("doc", true);
        }
        close_tag("attr", false);
    }

    void stats_if_elifs_else(If const & if_) {
        open_tag("if", false);
        open_tag("expr", false);
        expr(if_.expr);
        close_tag("expr", false);
        open_tag("then", false);
        stats(if_.then);
        close_tag("then", false);
        if (if_.else_) {
            open_tag("else", false);
            stats(if_.else_.get());
            close_tag("else", false);
        }
        close_tag("if", false);
    }

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
        for (unsigned int i = 0; i < level; i++) {
            os << " ";
        };
    }
    std::ostream & os;
    unsigned int level;
};

}

#endif
