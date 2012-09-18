#include <boost/spirit/include/support_istream_iterator.hpp>
#include <fstream>
#include <iostream>

#include "formast.hpp"
#include "formast/printer.hpp"

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

    formast::Expr ast;
    bool success = formast::parse_xml(in, ast);

    if (success) {
        formast::Printer print(std::cout);
        print.expr(ast);
    } else {
        std::cerr << "Error: Parsing failed\n";
    }
    return 0;
};
