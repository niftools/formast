#include "module_impl.hpp"

formast::Module::Impl::Impl()
    : std::vector<formast::detail::ModuleDecl>()
{
}

formast::Module::Module()
    : _impl(new Impl)
{
}
