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

    virtual void module_class(Class const & class_) {
        open_tag("class", false);
        open_tag("name", true);
        os << class_.name;
        close_tag("name", true);
        if (class_.has_template) {
            open_tag("has_template", true);
            close_tag("has_template", true);
        };
        if (class_.has_argument) {
            open_tag("has_argument", true);
            close_tag("has_argument", true);
        };
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

    virtual void module_enum(Enum const & enum_) {
        open_tag("enum", false);
        open_tag("name", true);
        os << enum_.name;
        close_tag("name", true);
        open_tag("base_name", true);
        os << enum_.base_name;
        close_tag("base_name", true);
        if (enum_.doc) {
            open_tag("doc", true);
            os << enum_.doc.get();
            close_tag("doc", true);
        }
        if (enum_.stats) {
            enum_stats(enum_.stats.get());
        }
        close_tag("enum", false);
    }

    virtual void stats(Stats const & s) {
        open_tag("stats", false);
        Visitor::stats(s);
        close_tag("stats", false);
    }

    virtual void stats_field(Field const & field) {
        open_tag("field", false);
        open_tag("type", true);
        os << field.type_;
        close_tag("type", true);
        if (field.template_) {
            open_tag("template", true);
            os << field.template_.get();
            close_tag("template", true);
        };
        if (field.argument) {
            open_tag("argument", true);
            os << field.argument.get();
            close_tag("argument", true);
        };
        open_tag("name", true);
        os << field.name;
        close_tag("name", true);
        if (field.doc) {
            open_tag("doc", true);
            os << field.doc.get();
            close_tag("doc", true);
        }
        close_tag("field", false);
    }

    virtual void stats_if(If const & if_) {
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

    virtual void enum_stats(EnumStats const & s) {
        open_tag("enum_stats", false);
        Visitor::enum_stats(s);
        close_tag("enum_stats", false);
    }

    virtual void enum_stats_const(EnumConst const & c) {
        open_tag("const", false);
        open_tag("name", true);
        os << c.name;
        close_tag("name", true);
        open_tag("value", true);
        os <<  boost::lexical_cast<std::string>(c.value);
        close_tag("value", true);
        if (c.doc) {
            open_tag("doc", true);
            os << c.doc.get();
            close_tag("doc", true);
        }
        close_tag("const", false);
    }

    virtual void expr_uint(boost::uint64_t const & n) {
        open_tag("uint", true);
        os << boost::lexical_cast<std::string>(n);
        close_tag("uint", true);
    }
    virtual void expr_id(std::string const & i) {
        open_tag("id", true);
        os << i;
        close_tag("id", true);
    }

    virtual void expr_pos(Expr const & right) {
        open_tag("pos", false);
        expr(right);
        close_tag("pos", false);
    }

    virtual void expr_neg(Expr const & right) {
        open_tag("neg", false);
        expr(right);
        close_tag("neg", false);
    }

    virtual void expr_logical_not(Expr const & right) {
        open_tag("logical_not", false);
        expr(right);
        close_tag("logical_not", false);
    }

    virtual void expr_add(Expr const & left, Expr const & right) {
        open_tag("add", false);
        expr(left);
        expr(right);
        close_tag("add", false);
    }

    virtual void expr_sub(Expr const & left, Expr const & right) {
        open_tag("sub", false);
        expr(left);
        expr(right);
        close_tag("sub", false);
    }

    virtual void expr_mul(Expr const & left, Expr const & right) {
        open_tag("mul", false);
        expr(left);
        expr(right);
        close_tag("mul", false);
    }

    virtual void expr_div(Expr const & left, Expr const & right) {
        open_tag("div", false);
        expr(left);
        expr(right);
        close_tag("div", false);
    }

    virtual void expr_mod(Expr const & left, Expr const & right) {
        open_tag("mod", false);
        expr(left);
        expr(right);
        close_tag("mod", false);
    }

    virtual void expr_pow(Expr const & left, Expr const & right) {
        open_tag("pow", false);
        expr(left);
        expr(right);
        close_tag("pow", false);
    }

    virtual void expr_logical_and(Expr const & left, Expr const & right) {
        open_tag("logical_and", false);
        expr(left);
        expr(right);
        close_tag("logical_and", false);
    }

    virtual void expr_logical_or(Expr const & left, Expr const & right) {
        open_tag("logical_or", false);
        expr(left);
        expr(right);
        close_tag("logical_or", false);
    }

    virtual void expr_bitwise_and(Expr const & left, Expr const & right) {
        open_tag("bitwise_and", false);
        expr(left);
        expr(right);
        close_tag("bitwise_and", false);
    }

    virtual void expr_bitwise_or(Expr const & left, Expr const & right) {
        open_tag("bitwise_or", false);
        expr(left);
        expr(right);
        close_tag("bitwise_or", false);
    }

    virtual void expr_bitwise_xor(Expr const & left, Expr const & right) {
        open_tag("bitwise_xor", false);
        expr(left);
        expr(right);
        close_tag("bitwise_xor", false);
    }

    virtual void expr_compare_eq(Expr const & left, Expr const & right) {
        open_tag("compare_eq", false);
        expr(left);
        expr(right);
        close_tag("compare_eq", false);
    }

    virtual void expr_compare_ne(Expr const & left, Expr const & right) {
        open_tag("compare_ne", false);
        expr(left);
        expr(right);
        close_tag("compare_ne", false);
    }

    virtual void expr_compare_gt(Expr const & left, Expr const & right) {
        open_tag("compare_gt", false);
        expr(left);
        expr(right);
        close_tag("compare_gt", false);
    }

    virtual void expr_compare_lt(Expr const & left, Expr const & right) {
        open_tag("compare_lt", false);
        expr(left);
        expr(right);
        close_tag("compare_lt", false);
    }

    virtual void expr_compare_ge(Expr const & left, Expr const & right) {
        open_tag("compare_ge", false);
        expr(left);
        expr(right);
        close_tag("compare_ge", false);
    }

    virtual void expr_compare_le(Expr const & left, Expr const & right) {
        open_tag("compare_le", false);
        expr(left);
        expr(right);
        close_tag("compare_le", false);
    }

    virtual void expr_shift_left(Expr const & left, Expr const & right) {
        open_tag("shift_left", false);
        expr(left);
        expr(right);
        close_tag("shift_left", false);
    }

    virtual void expr_shift_right(Expr const & left, Expr const & right) {
        open_tag("shift_right", false);
        expr(left);
        expr(right);
        close_tag("shift_right", false);
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
