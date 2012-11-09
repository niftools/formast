#include "formast.hpp"
#include "expr_impl.hpp"

formast::Expr::Impl::Impl()
    : tree()
{
}

formast::Expr::Expr()
    : _impl(new Impl)
{
}

formast::Expr::Expr(formast::Expr const & e)
    : _impl(new Impl)
{
    _impl->tree = e._impl->tree;
}

formast::Expr & formast::Expr::operator=(formast::Expr const & e)
{
    if (this != &e) {
        _impl->tree = e._impl->tree;
    }
    return *this;
}
