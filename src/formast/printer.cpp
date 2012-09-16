// implementation of a very basic AST stream printer

#include <boost/lexical_cast.hpp>

#include "formast/printer.hpp"

using namespace formast;

Printer::Printer(std::ostream & os) : Visitor<void>(), os(os), level(0) {};

void Printer::indent()
{
    for (int i = 0; i < level; i++) {
        os << " ";
    };
};

void Printer::open_tag(std::string const & tag, bool final)
{
    indent();
    os << "<" << tag << ">";
    if (!final) {
        os << std::endl;
        level++;
    }
}

void Printer::close_tag(std::string const & tag, bool final)
{
    if (!final) {
        level--;
        indent();
    }
    os << "</" << tag << ">" << std::endl;
}

void Printer::uint(boost::uint64_t const & n)
{
    open_tag("uint", true);
    os << boost::lexical_cast<std::string>(n);
    close_tag("uint", true);
}

void Printer::id(std::string const & i)
{
    open_tag("id", true);
    os << i;
    close_tag("id", true);
}

void Printer::pos(Expr const & right)
{
    open_tag("pos", false);
    expr(right);
    close_tag("pos", false);
}

void Printer::neg(Expr const & right)
{
    open_tag("neg", false);
    expr(right);
    close_tag("neg", false);
}

void Printer::add(Expr const & left, Expr const & right)
{
    open_tag("add", false);
    expr(left);
    expr(right);
    close_tag("add", false);
}

void Printer::sub(Expr const & left, Expr const & right)
{
    open_tag("sub", false);
    expr(left);
    expr(right);
    close_tag("sub", false);
}

void Printer::mul(Expr const & left, Expr const & right)
{
    open_tag("mul", false);
    expr(left);
    expr(right);
    close_tag("mul", false);
}

void Printer::div(Expr const & left, Expr const & right)
{
    open_tag("div", false);
    expr(left);
    expr(right);
    close_tag("div", false);
}
