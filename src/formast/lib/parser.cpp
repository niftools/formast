#include "parser_impl.hpp"

#include <boost/foreach.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <sstream>

#include "formast.hpp"
#include "enum_stats_impl.hpp"
#include "stats_impl.hpp"
#include "module_impl.hpp"

// upgrade structs to fusion sequences

BOOST_FUSION_ADAPT_STRUCT(
    formast::Field,
    (std::string, type_)
    (std::string, name)
    (boost::optional<formast::Doc>, doc)
)

BOOST_FUSION_ADAPT_STRUCT(
    formast::Class,
    (std::string, name)
    (boost::optional<std::string>, base_name)
    (boost::optional<formast::Doc>, doc)
    (boost::optional<formast::Stats>, stats)
)

BOOST_FUSION_ADAPT_STRUCT(
    formast::If,
    (formast::Expr, expr)
    (formast::Stats, then)
    (boost::optional<formast::Stats>, else_)
)

// parser implementation

formast::Parser::Impl::Impl() {};

// helper function for parsing expression from stream
void formast::Parser::Impl::_expr_parse_stream(std::istream & is, formast::Expr & e)
{
    // disable white space skipping
    is.unsetf(std::ios::skipws);
    Iterator iter(is);
    Iterator end;
    expr_grammar grammar;
    SpaceType space;

    bool success = boost::spirit::qi::phrase_parse(iter, end, grammar, space, e);
    if (!success) {
        throw std::runtime_error("Syntax error.");
    }
    if (iter != end) {
        throw std::runtime_error("End of stream not reached.");
    }
}

// helper function for parsing expression from string
void formast::Parser::Impl::_expr_xml_parse_string(std::string const & s, formast::Expr & e)
{
    std::istringstream is(s);
    _expr_parse_stream(is, e);
}

// parse s
// if expr contains nothing, assign parsed expression
// otherwise, && with parsed expression
void formast::Parser::Impl::_expr_xml_parse_helper(std::string const & s, formast::Expr & e)
{
    formast::Expr e2;
    _expr_xml_parse_string(s, e2);
    if (e._impl->tree) {
        // TODO find a way to use the phoenix function _logical_and
        binary_func<detail::binary_op::logical_and> const _logical_and_impl;
        _logical_and_impl(e, e2);
    } else {
        e._impl->tree = e2._impl->tree;
    }
}

// helper function for parsing integer from stream
void formast::Parser::Impl::_integer_parse_stream(std::istream & is, boost::uint64_t & v)
{
    // disable white space skipping
    is.unsetf(std::ios::skipws);
    Iterator iter(is);
    Iterator end;
    integer_grammar grammar;

    bool success = boost::spirit::qi::parse(iter, end, grammar, v);
    if (!success) {
        throw std::runtime_error("Syntax error.");
    }
    if (iter != end) {
        throw std::runtime_error("End of stream not reached.");
    }
}

// helper function for parsing integeression from string
void formast::Parser::Impl::_integer_xml_parse_string(std::string const & s, boost::uint64_t & e)
{
    std::istringstream is(s);
    _integer_parse_stream(is, e);
}

formast::Parser::Parser()
    : _impl(new Impl)
{
}

formast::Parser::~Parser()
{
}

void formast::Parser::parse_string(std::string const & s, formast::Module & module)
{
    std::istringstream is(s);
    parse_stream(is, module);
}

formast::XmlParser::XmlParser()
    : Parser()
{
}

// helper class to detect "ARG" in expressions
class ArgDetector : public formast::Visitor
{
public:
    ArgDetector() : formast::Visitor(), has_argument(false) {};

    bool has_argument;

    void expr(formast::Expr const & e) {
        if (!has_argument) {
            formast::Visitor::expr(e);
        }
    }

    void expr_id(std::string const & i) {
        if (i == "ARG") {
            has_argument = true;
        }
    }

    void expr_pos(formast::Expr const & right) {
        expr(right);
    }

    void expr_neg(formast::Expr const & right) {
        expr(right);
    }

    void expr_add(formast::Expr const & left, formast::Expr const & right) {
        expr(left);
        expr(right);
    }

    void expr_sub(formast::Expr const & left, formast::Expr const & right) {
        expr(left);
        expr(right);
    }

    void expr_mul(formast::Expr const & left, formast::Expr const & right) {
        expr(left);
        expr(right);
    }

    void expr_div(formast::Expr const & left, formast::Expr const & right) {
        expr(left);
        expr(right);
    }

    void expr_mod(formast::Expr const & left, formast::Expr const & right) {
        expr(left);
        expr(right);
    }

    void expr_pow(formast::Expr const & left, formast::Expr const & right) {
        expr(left);
        expr(right);
    }

    void expr_logical_and(formast::Expr const & left, formast::Expr const & right) {
        expr(left);
        expr(right);
    }

    void expr_logical_or(formast::Expr const & left, formast::Expr const & right) {
        expr(left);
        expr(right);
    }

    void expr_logical_not(formast::Expr const & right) {
        expr(right);
    }

    void expr_bitwise_and(formast::Expr const & left, formast::Expr const & right) {
        expr(left);
        expr(right);
    }

    void expr_bitwise_or(formast::Expr const & left, formast::Expr const & right) {
        expr(left);
        expr(right);
    }

    void expr_bitwise_xor(formast::Expr const & left, formast::Expr const & right) {
        expr(left);
        expr(right);
    }

    void expr_compare_eq(formast::Expr const & left, formast::Expr const & right) {
        expr(left);
        expr(right);
    }

    void expr_compare_ne(formast::Expr const & left, formast::Expr const & right) {
        expr(left);
        expr(right);
    }

    void expr_compare_gt(formast::Expr const & left, formast::Expr const & right) {
        expr(left);
        expr(right);
    }

    void expr_compare_lt(formast::Expr const & left, formast::Expr const & right) {
        expr(left);
        expr(right);
    }

    void expr_compare_ge(formast::Expr const & left, formast::Expr const & right) {
        expr(left);
        expr(right);
    }

    void expr_compare_le(formast::Expr const & left, formast::Expr const & right) {
        expr(left);
        expr(right);
    }

    void expr_shift_left(formast::Expr const & left, formast::Expr const & right) {
        expr(left);
        expr(right);
    }

    void expr_shift_right(formast::Expr const & left, formast::Expr const & right) {
        expr(left);
        expr(right);
    }
};

void formast::XmlParser::parse_stream(std::istream & is, formast::Module & module)
{
    // disable skipping of whitespace
    is.unsetf(std::ios::skipws);

    // read xml into property tree
    boost::property_tree::ptree pt;
    boost::property_tree::xml_parser::read_xml(is, pt);
    //boost::property_tree::info_parser::write_info(std::cout, pt); // DEBUG

    BOOST_FOREACH(boost::property_tree::ptree::value_type & decl, pt.get_child("niftoolsxml")) {
        if (decl.first == "basic") {
            Class class_;
            class_.name = decl.second.get<std::string>("<xmlattr>.name");
            std::string doc = decl.second.data();
            boost::algorithm::trim(doc);
            if (!doc.empty()) {
                class_.doc = doc;
            }
            module._impl->push_back(class_);
        } else if (decl.first == "enum") {
            Enum enum_;
            enum_.name = decl.second.get<std::string>("<xmlattr>.name");
            enum_.base_name = decl.second.get<std::string>("<xmlattr>.storage");
            std::string doc = decl.second.data();
            boost::algorithm::trim(doc);
            if (!doc.empty()) {
                enum_.doc = doc;
            }
            formast::EnumStats stats;
            BOOST_FOREACH(boost::property_tree::ptree::value_type & option, decl.second) {
                if (option.first == "option") {
                    EnumConst const_;
                    const_.name = option.second.get<std::string>("<xmlattr>.name");
                    _impl->_integer_xml_parse_string(option.second.get<std::string>("<xmlattr>.value"), const_.value);
                    std::string doc = option.second.data();
                    boost::algorithm::trim(doc);
                    if (!doc.empty()) {
                        const_.doc = doc;
                    }
                    stats._impl->push_back(const_);
                }
            }
            if (!stats._impl->empty()) {
                enum_.stats = stats;
            }
            module._impl->push_back(enum_);
        } else if (decl.first == "compound" || decl.first == "niobject") {
            Class class_;
            class_.name = decl.second.get<std::string>("<xmlattr>.name");
            std::string doc = decl.second.data();
            boost::algorithm::trim(doc);
            if (!doc.empty()) {
                class_.doc = doc;
            }
            class_.base_name = decl.second.get_optional<std::string>("<xmlattr>.inherit");
            formast::Stats stats;
            class_.has_template = false;
            class_.has_argument = false;
            ArgDetector arg_detector;
            BOOST_FOREACH(boost::property_tree::ptree::value_type & add, decl.second) {
                if (add.first == "add") {
                    Field field;
                    field.type_ = add.second.get<std::string>("<xmlattr>.type");
                    if (field.type_ == "TEMPLATE") {
                        class_.has_template = true;
                    }
                    field.name = add.second.get<std::string>("<xmlattr>.name");
                    field.template_ = add.second.get_optional<std::string>("<xmlattr>.template");
                    if (field.template_) {
                        if (field.template_.get() == "TEMPLATE") {
                            class_.has_template = true;
                        }
                    }
                    field.argument = add.second.get_optional<std::string>("<xmlattr>.arg");
                    std::string doc = add.second.data();
                    boost::algorithm::trim(doc);
                    if (!doc.empty()) {
                        field.doc = doc;
                    }
                    boost::optional<std::string> arr1 =
                        add.second.get_optional<std::string>("<xmlattr>.arr1");
                    if (arr1) {
                        Expr e;
                        _impl->_expr_xml_parse_string(arr1.get(), e);
                        field.arr1 = e;
                        arg_detector.expr(e);
                    }
                    boost::optional<std::string> arr2 =
                        add.second.get_optional<std::string>("<xmlattr>.arr2");
                    if (arr2) {
                        Expr e;
                        _impl->_expr_xml_parse_string(arr2.get(), e);
                        field.arr2 = e;
                        arg_detector.expr(e);
                    }
                    boost::optional<std::string> ver1 =
                        add.second.get_optional<std::string>("<xmlattr>.ver1");
                    boost::optional<std::string> ver2 =
                        add.second.get_optional<std::string>("<xmlattr>.ver2");
                    boost::optional<std::string> userver =
                        add.second.get_optional<std::string>("<xmlattr>.userver");
                    boost::optional<std::string> userver2 =
                        add.second.get_optional<std::string>("<xmlattr>.userver2");
                    boost::optional<std::string> vercond =
                        add.second.get_optional<std::string>("<xmlattr>.vercond");
                    boost::optional<std::string> cond =
                        add.second.get_optional<std::string>("<xmlattr>.cond");
                    Expr e;
                    if (cond) _impl->_expr_xml_parse_helper(cond.get(), e);
                    if (vercond) _impl->_expr_xml_parse_helper(vercond.get(), e);
                    if (ver1)
                        _impl->_expr_xml_parse_helper(
                            "Version >= " + ver1.get(), e);
                    if (ver2)
                        _impl->_expr_xml_parse_helper(
                            "Version <= " + ver2.get(), e);
                    if (userver)
                        _impl->_expr_xml_parse_helper(
                            "User Version == " + userver.get(), e);
                    if (userver2)
                        _impl->_expr_xml_parse_helper(
                            "User Version 2 == " + userver2.get(), e);
                    if (e._impl->tree) {
                        formast::If if_;
                        if_.expr = e;
                        if_.then._impl->push_back(field);
                        stats._impl->push_back(if_);
                        arg_detector.expr(e);
                    } else {
                        stats._impl->push_back(field);
                    }
                };
            };
            if (!stats._impl->empty()) {
                class_.stats = stats;
            };
            class_.has_argument = arg_detector.has_argument;
            module._impl->push_back(class_);
        };
    };
}

boost::phoenix::function<formast::Parser::Impl::error_handler_> const formast::Parser::Impl::error_handler = formast::Parser::Impl::error_handler_();
