#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <boost/lexical_cast.hpp>
#include<sstream>

#include "formast.hpp"
#include "printer.hpp"

// disable indentation and newlines for simpler inline tests
class TestPrinter : public formast::Printer
{
public:
    TestPrinter(std::ostream & os) : formast::Printer(os) {};
    void indent() {};
    void open_tag(std::string const & tag, bool final) {
        os << "<" << tag << ">";
    };
    void close_tag(std::string const & tag, bool final) {
        os << "</" << tag << ">";
    };
};

class ParseFixture
{
public:
    ParseFixture() {};
    void parse_check(std::string const & in, std::string const & out) const {
        formast::Top ast;
        formast::XmlParser parser;
        BOOST_CHECK_NO_THROW(parser.parse_string(in, ast));

        std::ostringstream os;
        TestPrinter print(os);
        print.top(ast);
        BOOST_CHECK_EQUAL(os.str(), out);
    };
};

BOOST_AUTO_TEST_SUITE(expr_parse_xml_test_suite)

BOOST_FIXTURE_TEST_CASE(uint_test, ParseFixture)
{
    parse_check("31415", "<uint>31415</uint>");
}

BOOST_FIXTURE_TEST_CASE(unary_test, ParseFixture)
{
    parse_check("+31415", "<pos><uint>31415</uint></pos>");
    parse_check("-31415", "<neg><uint>31415</uint></neg>");
}

BOOST_FIXTURE_TEST_CASE(binary_test, ParseFixture)
{
    parse_check("1+2", "<add><uint>1</uint><uint>2</uint></add>");
    parse_check("3-4", "<sub><uint>3</uint><uint>4</uint></sub>");
    parse_check("7*8", "<mul><uint>7</uint><uint>8</uint></mul>");
    parse_check("19/33", "<div><uint>19</uint><uint>33</uint></div>");
}

BOOST_FIXTURE_TEST_CASE(complex_test, ParseFixture)
{
    parse_check("1*2+6/3*7-9*(1+2-3-4)", "<sub><add><mul><uint>1</uint><uint>2</uint></mul><mul><div><uint>6</uint><uint>3</uint></div><uint>7</uint></mul></add><mul><uint>9</uint><sub><sub><add><uint>1</uint><uint>2</uint></add><uint>3</uint></sub><uint>4</uint></sub></mul></sub>");
}

BOOST_AUTO_TEST_SUITE_END()
