#pragma once

#include <cstdint>
#include <vector>
#include <istream>
#include <exception>

// An exception that is thrown by evaluate when it detects an incorrect expression
class incorrect_expression : public std::invalid_argument {
public:
    explicit incorrect_expression(const std::string &what_arg)
            : invalid_argument(what_arg) {
    }
};

double evaluate(const char *, std::istream &);