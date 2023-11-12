#include <iostream>

#include "tokenizer.h"
#include "parser.h"

parser::parser(const std::string &input, const operations &ops)
        : m_tokenizer{input}, m_ops{ops} {
    // Prepare first token on start.
    (void) m_tokenizer.next_token();
}

double parser::eval_atom() {
    const auto *curr_tok = m_tokenizer.curr_token();
    if (const auto *as_lparen_tok = dynamic_cast<const tokenizer::lparen_token *>(curr_tok);
            as_lparen_tok != nullptr) {
        (void) m_tokenizer.next_token();
        const auto val = eval(1);
        if (const auto *as_rparen_tok = dynamic_cast<const tokenizer::rparen_token *>(m_tokenizer.curr_token());
                as_rparen_tok == nullptr)
            throw syntax_error{"Unmatched parentheses."};
        (void) m_tokenizer.next_token();
        return val;
    }

    if (!curr_tok)
        throw syntax_error{"Unexpected eof."};

    if (const auto *as_op_tok = dynamic_cast<const tokenizer::op_token *>(curr_tok);
            as_op_tok != nullptr)
        throw syntax_error{"Unexpected operation."};

    if (const auto *as_num_tok = dynamic_cast<const tokenizer::num_token *>(curr_tok);
            as_num_tok != nullptr) {
        const int curr_num = as_num_tok->num;
        (void) m_tokenizer.next_token();
        return curr_num;
    }

    if (const auto *as_rparen_tok = dynamic_cast<const tokenizer::rparen_token *>(curr_tok); as_rparen_tok != nullptr)
        throw syntax_error{"Missing opening paren."};

    // Safety: All token types have been tried.
    abort();
}

double parser::eval_op(const op_descriptor &op_desc, double lhs, double rhs) {
    // clang-format off
    switch (op_desc.action) {
        case op_descriptor::op_action::Add:
            return lhs + rhs;
        case op_descriptor::op_action::Sub:
            return lhs - rhs;
        case op_descriptor::op_action::Mul:
            return lhs * rhs;
        case op_descriptor::op_action::Div:
            return lhs / rhs;
    }
    // clang-format on

    // Safety: op_desc.action values are exhausted in the switch.
    __builtin_unreachable();
}

double parser::eval(double min_priority) {
    if (m_tokenizer.eof() && min_priority == 1)
        return 0;

    double atom_lhs = eval_atom();
    for (; m_tokenizer.curr_token();) {
        const tokenizer::base_token *curr_token = m_tokenizer.curr_token();
        if (const auto *as_num_tok = dynamic_cast<const tokenizer::num_token *>(curr_token); as_num_tok != nullptr)
            throw syntax_error{"Two consecutive numbers"};

        const auto *as_op_tok = dynamic_cast<const tokenizer::op_token *>(curr_token);
        if (!as_op_tok)
            break;

        // This throws `invalid_operation` in case such operation does not exist in the registered set.
        const auto &op_desc = m_ops.op_by_name(as_op_tok->name);
        if (op_desc.priority < min_priority)
            break;

        const size_t next_min_priority = op_desc.associativity == op_descriptor::op_associativity::Left
                                         ? op_desc.priority + 1
                                         : op_desc.priority;

        (void) m_tokenizer.next_token();
        const double atom_rhs = eval(next_min_priority);
        atom_lhs = eval_op(op_desc, atom_lhs, atom_rhs);
    }

    return atom_lhs;
}