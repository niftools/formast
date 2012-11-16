#ifndef FORMAST_HPP_INCLUDED
#define FORMAST_HPP_INCLUDED

#ifdef FORMAST_STATIC
// formast static library
#  define FORMAST_API
#  define FORMAST_HIDDEN
#else
// formast shared library
#  if defined _WIN32 || defined __CYGWIN__
// Windows
#    ifdef formastlib_EXPORTS
// formast is being compiled: export API to dll
#      define FORMAST_API __declspec(dllexport)
#    else
// other code is being compiled, using formast.dll: import API from dll
#      define FORMAST_API __declspec(dllimport)
#    endif
#    define FORMAST_HIDDEN
#  elif __GNUC__ >= 4
// GNU Linux: set visibility of symbols
#    define FORMAST_API    __attribute__ ((visibility ("default")))
#    define FORMAST_HIDDEN __attribute__ ((visibility ("hidden")))
#  else
// unknown
#    define FORMAST_API
#    define FORMAST_HIDDEN
#  endif
#endif

#include <boost/cstdint.hpp>
#include <boost/optional.hpp>
#include <memory>
#include <string>
#include <vector>

namespace formast
{

class Module
{
public:
    FORMAST_API Module();
private:
    // pimpl idiom
    FORMAST_HIDDEN class Impl;
    std::shared_ptr<Impl> _impl;
    friend class Visitor;
    friend class Parser;
    friend class XmlParser;
};

class Stats
{
public:
    FORMAST_API Stats();
private:
    // pimpl idiom
    FORMAST_HIDDEN class Impl;
    std::shared_ptr<Impl> _impl;
    friend class Visitor;
    friend class Parser;
    friend class XmlParser;
};

class Expr
{
public:
    FORMAST_API Expr();
    FORMAST_API Expr(Expr const & e);
    FORMAST_API Expr & operator=(Expr const & e);
private:
    // pimpl idiom
    FORMAST_HIDDEN class Impl;
    std::shared_ptr<Impl> _impl;
    friend class Visitor;
    friend class Parser;
    friend class XmlParser;
};

typedef std::string Doc;

class Field
{
public:
    std::string type_;
    boost::optional<std::string> template_;
    boost::optional<std::string> argument;
    std::string name;
    boost::optional<Doc> doc;
    boost::optional<Expr> arr1;
    boost::optional<Expr> arr2;
};

class Class
{
public:
    std::string name;
    bool has_template;
    bool has_argument;
    boost::optional<std::string> base_name;
    boost::optional<Doc> doc;
    boost::optional<Stats> stats;
};

class If
{
public:
    Expr expr;
    Stats then;
    boost::optional<Stats> else_;
};

class EnumConst
{
public:
    std::string name;
    boost::uint64_t value; // TODO allow signed, use boost::multiprecision:cpp_int
    boost::optional<Doc> doc;
};

class EnumStats
{
public:
    FORMAST_API EnumStats();
private:
    // pimpl idiom
    FORMAST_HIDDEN class Impl;
    std::shared_ptr<Impl> _impl;
    friend class Visitor;
    friend class Parser;
    friend class XmlParser;
};

class Enum
{
public:
    std::string name;
    std::string base_name;
    boost::optional<Doc> doc;
    boost::optional<EnumStats> stats;
};

class Parser
{
public:
    FORMAST_API Parser();
    FORMAST_API virtual ~Parser();
    FORMAST_API virtual void parse_stream(std::istream & is, Module & module) = 0;
    FORMAST_API void parse_string(std::string const & s, Module & module);
private:
    // pimpl idiom
    FORMAST_HIDDEN class Impl;
    std::shared_ptr<Impl> _impl;
    friend class XmlParser;
};

class XmlParser : public Parser
{
public:
    FORMAST_API XmlParser();
    FORMAST_API virtual void parse_stream(std::istream & is, Module & module);
};

class Visitor
{
public:
    FORMAST_API Visitor();
    FORMAST_API virtual ~Visitor();

    FORMAST_API virtual void module(Module const & module);
    FORMAST_API virtual void module_class(Class const & class_);
    FORMAST_API virtual void module_enum(Enum const & enum_);

    FORMAST_API virtual void stats(Stats const & stats);
    FORMAST_API virtual void stats_field(Field const & field);
    FORMAST_API virtual void stats_if(If const & if_);

    FORMAST_API virtual void enum_stats(EnumStats const & stats);
    FORMAST_API virtual void enum_stats_const(EnumConst const & const_);

    FORMAST_API virtual void expr(Expr const & e);
    FORMAST_API virtual void expr_uint(boost::uint64_t const & n); // TODO use unsigned boost::multiprecision::cpp_int
    FORMAST_API virtual void expr_id(std::string const & i);
    FORMAST_API virtual void expr_pos(Expr const & right);
    FORMAST_API virtual void expr_neg(Expr const & right);
    FORMAST_API virtual void expr_add(Expr const & left, Expr const & right);
    FORMAST_API virtual void expr_sub(Expr const & left, Expr const & right);
    FORMAST_API virtual void expr_mul(Expr const & left, Expr const & right);
    FORMAST_API virtual void expr_div(Expr const & left, Expr const & right);
    FORMAST_API virtual void expr_mod(Expr const & left, Expr const & right);
    FORMAST_API virtual void expr_pow(Expr const & left, Expr const & right);
    FORMAST_API virtual void expr_logical_and(Expr const & left, Expr const & right);
    FORMAST_API virtual void expr_logical_or(Expr const & left, Expr const & right);
    FORMAST_API virtual void expr_logical_not(Expr const & right);
    FORMAST_API virtual void expr_bitwise_and(Expr const & left, Expr const & right);
    FORMAST_API virtual void expr_bitwise_or(Expr const & left, Expr const & right);
    FORMAST_API virtual void expr_bitwise_xor(Expr const & left, Expr const & right);
    FORMAST_API virtual void expr_compare_eq(Expr const & left, Expr const & right);
    FORMAST_API virtual void expr_compare_ne(Expr const & left, Expr const & right);
    FORMAST_API virtual void expr_compare_gt(Expr const & left, Expr const & right);
    FORMAST_API virtual void expr_compare_lt(Expr const & left, Expr const & right);
    FORMAST_API virtual void expr_compare_ge(Expr const & left, Expr const & right);
    FORMAST_API virtual void expr_compare_le(Expr const & left, Expr const & right);
    FORMAST_API virtual void expr_shift_left(Expr const & left, Expr const & right);
    FORMAST_API virtual void expr_shift_right(Expr const & left, Expr const & right);
private:
    // pimpl idiom
    FORMAST_HIDDEN class ExprVisitor;
    std::shared_ptr<ExprVisitor> _expr_visitor;
    FORMAST_HIDDEN class ModuleVisitor;
    std::shared_ptr<ModuleVisitor> _module_visitor;
    FORMAST_HIDDEN class StatsVisitor;
    std::shared_ptr<StatsVisitor> _stats_visitor;
    FORMAST_HIDDEN class EnumStatsVisitor;
    std::shared_ptr<EnumStatsVisitor> _enum_stats_visitor;
};

} // namespace formast

#endif
