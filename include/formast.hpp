#ifndef FORMAST_HPP_INCLUDED
#define FORMAST_HPP_INCLUDED

#ifdef FORMAST_STATIC
  // formast static library
  #define FORMAST_API
  #define FORMAST_HIDDEN
#else
  // formast shared library
  #if defined _WIN32 || defined __CYGWIN__
    // Windows
    #ifdef formastlib_EXPORTS
      // formast is being compiled: export API to dll
      #define FORMAST_API __declspec(dllexport)
    #else
      // other code is being compiled, using formast.dll: import API from dll
      #define FORMAST_API __declspec(dllimport)
    #endif
    #define FORMAST_HIDDEN
  #elif __GNUC__ >= 4
    // GNU Linux: set visibility of symbols
    #define FORMAST_API    __attribute__ ((visibility ("default")))
    #define FORMAST_HIDDEN __attribute__ ((visibility ("hidden")))
  #else
    // unknown
    #define FORMAST_API
    #define FORMAST_HIDDEN
  #endif
#endif

#include <boost/cstdint.hpp>
#include <boost/shared_ptr.hpp>
#include <string>

#include "formast/detail/ast.hpp"

namespace formast
{

typedef formast::detail::ast::Expr Expr;

class Parser
{
public:
    FORMAST_API Parser();
    FORMAST_API virtual Expr parse_stream(std::istream & is) = 0;
    FORMAST_API Expr parse_string(std::string const & s);
};

class XmlParser : public Parser
{
public:
    FORMAST_API XmlParser();
    FORMAST_API virtual Expr parse_stream(std::istream & is);
};

class Visitor
{
public:
    FORMAST_API Visitor();
    FORMAST_API virtual ~Visitor();
    FORMAST_API virtual void expr(Expr const & e);
    FORMAST_API virtual void expr_uint(boost::uint64_t const & n);
    FORMAST_API virtual void expr_id(std::string const & i);
    FORMAST_API virtual void expr_pos(Expr const & right);
    FORMAST_API virtual void expr_neg(Expr const & right);
    FORMAST_API virtual void expr_add(Expr const & left, Expr const & right);
    FORMAST_API virtual void expr_sub(Expr const & left, Expr const & right);
    FORMAST_API virtual void expr_mul(Expr const & left, Expr const & right);
    FORMAST_API virtual void expr_div(Expr const & left, Expr const & right);
private:
    // pimpl idiom
    FORMAST_HIDDEN class ExprVisitor;
    FORMAST_HIDDEN boost::shared_ptr<ExprVisitor> _expr_visitor;
};

} // namespace formast

#endif
