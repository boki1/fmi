#include <cassert>
#include "tokenizer.h"

bool tokenizer::is_small_letter(char s) {
    const char ss = std::tolower(s);
    return 'a' <= ss && ss <= 'z';
}

tokenizer::tokenizer(const std::string &input)
        : m_input_ss{input}, m_current_token{nullptr} {}

mystd::scoped_ptr<tokenizer::base_token> tokenizer::consume_next_token() {
    char peeked = m_input_ss.peek();
    bool is_negative = false;

    for (; std::isspace(peeked) && !m_input_ss.eof(); peeked = m_input_ss.peek())
        m_input_ss.get();

    if (m_input_ss.eof())
        return nullptr;

    if (is_small_letter(peeked)) {
        const char _ = m_input_ss.get();
        assert(is_small_letter(_));
        return mystd::make_scoped<op_token>(std::tolower(peeked));
    }

    if (peeked == '-') {
        is_negative = true;
        const char _ = m_input_ss.get();
        assert(_ == '-');
        peeked = m_input_ss.peek();
    }

    if (std::isdigit(peeked)) {
        int num;
        m_input_ss >> num;
        return mystd::make_scoped<num_token>(is_negative ? -num : num);
    } else if (is_negative)
        throw syntax_error{"Unexpected whitespace after sign symbol."};

    if (peeked == '(') {
        m_input_ss.get();
        return mystd::make_scoped<lparen_token>();
    }

    if (peeked == ')') {
        m_input_ss.get();
        return mystd::make_scoped<rparen_token>();
    }

    throw syntax_error{"Unknown symbol"};
}