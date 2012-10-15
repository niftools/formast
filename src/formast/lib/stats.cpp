#include "stats_impl.hpp"

formast::Stats::Impl::Impl()
    : std::vector<formast::detail::StatDecl>()
{
}

formast::Stats::Stats()
    : _impl(new Impl)
{
}
