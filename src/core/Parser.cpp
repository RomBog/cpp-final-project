#include "core/Parser.h"

Parser::Parser(int argc, char* argv[]) {
    Parse(argc, argv);
}

constexpr int Cargc = 3;

void Parser::Parse(int argc, char* argv[]) {
    if (argc < Cargc) {
        throw std::invalid_argument("Not enough arguments");
    }

    input_path_ = argv[1];
    output_path_ = argv[2];

    FilterDescriptor current_filter;
    for (size_t i = 3; i < argc; i++) {
        std::string_view arg = argv[i];

        if (arg[0] != '-') {
            if (current_filter.name.empty()) {
                throw std::invalid_argument("No filter name specified");
            }
            current_filter.args.push_back(argv[i]);
        } else {
            if (!current_filter.name.empty()) {
                filters_.push_back(current_filter);
            }
            current_filter.args.clear();
            current_filter.name = arg.substr(1);
        }
    }
    if (!current_filter.name.empty()) {
        filters_.push_back(current_filter);
    }
}