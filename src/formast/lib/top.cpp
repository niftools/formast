#include "formast.hpp"
#include "top_impl.hpp"

formast::Top::Impl::Impl()
    : std::vector<formast::detail::TopDecl>()
{
}

formast::Top::Top()
    : _impl(new Impl)
{
}
