#include "enum_stats_impl.hpp"

formast::EnumStats::Impl::Impl()
    : std::vector<formast::detail::EnumStatDecl>()
{
}

formast::EnumStats::EnumStats()
    : _impl(new Impl)
{
}
