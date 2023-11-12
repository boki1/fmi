#ifndef HOMEWORK_1_EXPRESSION_PROCESSING_OPERATIONS_H_INCLUDED
#define HOMEWORK_1_EXPRESSION_PROCESSING_OPERATIONS_H_INCLUDED

#include <cstdint>
#include <vector>
#include <istream>
#include <cassert>
#include <exception>

inline bool is_sletter(char sym) {
    return 'a' <= sym && sym <= 'z';
}

inline size_t sletter2index(char sym) {
    assert('a' <= sym && sym <= 'z');
    return sym - 'a';
}

class invalid_operation : public std::invalid_argument {
public:
    explicit invalid_operation(const std::string &what_arg)
            : invalid_argument(what_arg) {
    }
};

struct op_descriptor {
    char name;
    size_t priority;

    enum class op_associativity {
        Right, Left
    } associativity;

    enum class op_action {
        Add, Sub, Mul, Div
    } action;

    /*
     * The string format of an operator descriptor looks like this:
     * <symbol> <operator> <priority> <associativity>
     * where
     *  - symbol is a (caseless) ASCII letter,
     *  - operator is one of +, -, /, *,
     *  - priority is an unsigned integer - the bigger the value, the higher the priority,
     *  - associativity is either left or right.
     */
    explicit op_descriptor(std::istream &);

    [[nodiscard]] std::string to_string() const {
        static const char *assoc_names[] = {"left", "right"};
        static const char *op_action_names = "+-/*";

        std::ostringstream oss;
        oss << name << " [ " << priority << ", " << assoc_names[static_cast<int>(associativity)]
            << op_action_names[static_cast<int>(action)] << " ]";
        return oss.str();
    }
};

class operations {
    static const inline size_t expected_op_count = 'z' - 'a' + 1;

public: // Object lifetime
    explicit operations(std::istream &ifs);

public: // Operations
    const op_descriptor &op_by_name(char name) const;

    bool op_is_valid(char name) const noexcept;

private: // Members
    std::vector<op_descriptor> m_op_descriptors{};
    uint32_t m_op_valid{0};
};


#endif //HOMEWORK_1_EXPRESSION_PROCESSING_OPERATIONS_H_INCLUDED
