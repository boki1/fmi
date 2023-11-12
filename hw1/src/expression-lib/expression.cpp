#include <sstream>
#include <cassert>
#include <fstream>

#include "expression.h"
#include "parser.h"
#include "operations.h"

void token_printer(const std::string &out_file, const std::string &input) {
    tokenizer t{input};
    std::ofstream ofs{out_file, std::ios_base::app | std::ios_base::out};
    ofs << "Input '" << input << "'\n\n";

    try {
        while (auto &tok = t.next_token()) {
            ofs << tok->to_string() << '\n';
        }
    } catch (const syntax_error &se) {
        ofs << "Error: " << se.what() << '\n';
    }

    ofs << "------------\n\n";
}

///
/// @brief Evaluates an expression.
///
/// @param expression
///   A null-terminated string that contains the expression.
/// @param ops
///   An input stream which contains the descriptions of all operations used in the expression.
///
/// @return The calculated value of the expression
///
double evaluate(const char *expression, std::istream &ops_file) {
    if (expression == nullptr)
        throw incorrect_expression("Nullptr passed");
    const operations ops{ops_file};
#if 0
    token_printer("debug-output", expression);
#endif
    
    try {
        parser p{std::string{expression}, ops};
        return p.eval();
    } catch (const invalid_operation &io) {
        throw incorrect_expression{io.what()};
    } catch (const syntax_error &se) {
        throw incorrect_expression{se.what()};
    }
}