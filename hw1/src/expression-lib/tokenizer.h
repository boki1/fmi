#ifndef HOMEWORK_1_EXPRESSION_PROCESSING_TOKENIZER_H_INCLUDED
#define HOMEWORK_1_EXPRESSION_PROCESSING_TOKENIZER_H_INCLUDED

#include <sstream>
#include "scoped_ptr.h"

class syntax_error : public std::runtime_error {
public:
    explicit syntax_error(const std::string &what_arg)
            : runtime_error(what_arg) {
    }
};

class tokenizer final {
public: // Token type hierarchy.
    struct base_token {
        virtual ~base_token() noexcept = default;

        virtual std::string to_string() const = 0;
    };

    struct num_token : public base_token {
        explicit num_token(const int t_num) : num{t_num} {}

        ~num_token() noexcept override = default;

        [[nodiscard]] std::string to_string() const override { return std::to_string(num); };
        const int num;
    };

    struct op_token : public base_token {
        explicit op_token(const char t_name) : name{t_name} {}

        ~op_token() noexcept override = default;

        [[nodiscard]] std::string to_string() const override {
            return std::string(1, name);
        };

        const char name;
    };

    struct lparen_token : public base_token {
        ~lparen_token() noexcept override = default;

        [[nodiscard]] std::string to_string() const override { return "("; };
    };

    struct rparen_token : public base_token {
        ~rparen_token() noexcept override = default;

        [[nodiscard]] std::string to_string() const override { return ")"; };
    };

public: // Helper
    static bool is_small_letter(char s);

public: // Special member function
    explicit tokenizer(const std::string &);

private: // Operations
    /// Reads the next token and validates into m_current_token and returns its value.
    /// `m_current_token` stores it until the next invocation of `consume_next_token()` which
    /// overrides it.
    mystd::scoped_ptr<base_token> consume_next_token();

public: // Operation
    mystd::scoped_ptr<base_token> &next_token() {
        m_current_token = consume_next_token();
        return m_current_token;
    }

    const base_token *curr_token() const noexcept {
        return m_current_token.get();
    }

    bool eof() const {
        return m_input_ss.eof();
    }

private:
    mystd::scoped_ptr<base_token> m_current_token;
    std::istringstream m_input_ss;
};

#endif //HOMEWORK_1_EXPRESSION_PROCESSING_TOKENIZER_H_INCLUDED
