#include <boost/spirit/include/support_istream_iterator.hpp>
#include <fstream>
#include <iostream>

#include "formast.hpp"

int main(int argc, char **argv)
{
    char const* filename;
    if (argc > 1) {
        filename = argv[1];
    } else {
        std::cerr << "Error: No input file provided." << std::endl;
        return 1;
    }

    std::ifstream in(filename, std::ios_base::in);

    if (!in) {
        std::cerr << "Error: Could not open input file: "
                  << filename << std::endl;
        return 1;
    }

    // disable white space skipping
    in.unsetf(std::ios::skipws);

    typedef boost::spirit::istream_iterator Iterator;
    Iterator iter(in);
    Iterator end;
    formast::Expr ast;
    bool success = formast::parse_xml(iter, end, ast);

    if (success && iter == end) {
        // TODO print AST
    } else {
        std::cerr << "Error: Parsing failed\n";
    }
    return 0;
};
