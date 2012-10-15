#include <boost/spirit/include/support_istream_iterator.hpp>
#include <exception>
#include <fstream>
#include <iostream>

#include "formast.hpp"
#include "printer.hpp"

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

    formast::Module ast;
    formast::XmlParser parser;
    try {
        parser.parse_stream(in, ast);
    } catch (std::exception & e) {
        std::cerr << "Error: Parsing failed\n";
        std::cerr << e.what() << std::endl;
        return 1;
    }
    formast::Printer print(std::cout);
    print.top(ast);
    return 0;
};
