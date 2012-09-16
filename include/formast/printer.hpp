#ifndef FORMAST_PRINTER_HPP_INCLUDED
#define FORMAST_PRINTER_HPP_INCLUDED

#include <boost/noncopyable.hpp>
#include <ostream>
#include "formast.hpp"

namespace formast
{

class Printer : public Visitor<void>
{
public:
    Printer(std::ostream & os);
    void uint(boost::uint64_t const & n);
    void id(std::string const & i);
    void pos(Expr const & right);
    void neg(Expr const & right);
    void add(Expr const & left, Expr const & right);
    void sub(Expr const & left, Expr const & right);
    void mul(Expr const & left, Expr const & right);
    void div(Expr const & left, Expr const & right);
    void open_tag(std::string const & tag, bool final);
    void close_tag(std::string const & tag, bool final);
    void indent();
    std::ostream & os;
    unsigned int level;
};

}

#endif
