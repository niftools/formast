#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <boost/lexical_cast.hpp>

#include "formast.hpp"
#include "formast/printer.hpp"

class ParseFixture
{
public:
    ParseFixture() {};
    void parse_check(std::string const & in, std::string const & out) const {
        formast::Expr expr;
        std::string::const_iterator iter = in.begin();
        std::string::const_iterator end = in.end();
        BOOST_CHECK_EQUAL(formast::parse_xml(iter, end, expr), true);

        std::ostringstream os;
        formast::Printer print(os);
        print.expr(expr);
        BOOST_CHECK_EQUAL(os.str(), out);
    };
};

BOOST_AUTO_TEST_SUITE(expr_parse_xml_test_suite)

BOOST_FIXTURE_TEST_CASE(uint_test, ParseFixture)
{
    parse_check("31415", "<uint>31415</uint>\n");
}

BOOST_FIXTURE_TEST_CASE(unary_test, ParseFixture)
{
    parse_check("+31415", "<pos>\n <uint>31415</uint>\n</pos>\n");
    parse_check("-31415", "<neg>\n <uint>31415</uint>\n</neg>\n");
}

BOOST_AUTO_TEST_SUITE_END()
