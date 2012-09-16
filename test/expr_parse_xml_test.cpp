#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <boost/lexical_cast.hpp>

#include "formast.hpp"

class Printer : public formast::Visitor<std::string>
{
public:
    std::string uint(boost::uint64_t const & n) const {
        return "<uint>" + boost::lexical_cast<std::string>(n) + "</uint>";
    };
    std::string id(std::string const & i) const {
        return "<id>" + i + "</id>";
    };
    std::string pos(formast::Expr const & right) const {
        return "<pos>" + expr(right) + "</pos>";
    };
    std::string neg(formast::Expr const & right) const {
        return "<neg>" + expr(right) + "</neg>";
    };
    std::string add(formast::Expr const & left, formast::Expr const & right) const {
        return "<add>" + expr(left) + expr(right) + "</add>";
    };
    std::string sub(formast::Expr const & left, formast::Expr const & right) const {
        return "<sub>" + expr(left) + expr(right) + "</sub>";
    };
    std::string mul(formast::Expr const & left, formast::Expr const & right) const {
        return "<mul>" + expr(left) + expr(right) + "</mul>";
    };
    std::string div(formast::Expr const & left, formast::Expr const & right) const {
        return "<div>" + expr(left) + expr(right) + "</div>";
    };
};

class ParseFixture
{
public:
    ParseFixture() : print() {};
    void parse_check(std::string const & in, std::string const & out) const {
        formast::Expr expr;
        std::string::const_iterator iter = in.begin();
        std::string::const_iterator end = in.end();
        BOOST_CHECK_EQUAL(formast::parse_xml(iter, end, expr), true);
        BOOST_CHECK_EQUAL(print.expr(expr), out);
    };
    Printer print;
};

BOOST_AUTO_TEST_SUITE(expr_parse_xml_test_suite)

BOOST_FIXTURE_TEST_CASE(uint_test, ParseFixture)
{
    parse_check("31415", "<uint>31415</uint>");
}

BOOST_AUTO_TEST_SUITE_END()
