#pragma once

#include <string>
#include <vector>
#include <stdexcept>

#include "FilterDescriptor.h"

class Parser {
public:
    Parser(int argc, char* argv[]);

    const std::string& GetInputPath() const {
        return input_path_;
    }

    const std::string& GetOutputPath() const {
        return output_path_;
    }

    const std::vector<FilterDescriptor>& GetFilters() const {
        return filters_;
    }

private:
    std::string input_path_;
    std::string output_path_;
    std::vector<FilterDescriptor> filters_;

    void Parse(int argc, char* argv[]);
};
