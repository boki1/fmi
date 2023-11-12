#include <sstream>
#include <cassert>

#include "operations.h"

operations::operations(std::istream &ifs) {
    m_op_descriptors.reserve(expected_op_count);
    ifs >> std::ws;
    for (; !ifs.eof() && ifs.good(); ifs >> std::ws) {
        op_descriptor desc{ifs};
        const size_t index = sletter2index(desc.name);
        m_op_descriptors[index] = std::move(desc);
        m_op_valid |= 1 << index;
    }
}

bool operations::op_is_valid(char name) const noexcept {
    return is_sletter(name) && m_op_valid & (1 << sletter2index(name));
}

const op_descriptor &operations::op_by_name(char name) const {
    if (op_is_valid(name))
        return m_op_descriptors[sletter2index(name)];
    throw invalid_operation("Trying to use unregistered operation.");
}

op_descriptor::op_descriptor(std::istream &ifs) {
    name = [&]() {
        char tmp;
        ifs >> tmp;
        return std::tolower(tmp);
    }();
    action = [&]() {
        char tmp;
        switch (ifs >> tmp; tmp) {
            case '-':
                return op_action::Sub;
            case '+':
                return op_action::Add;
            case '*':
                return op_action::Mul;
            case '/':
                return op_action::Div;
            default:
                // Safety:
                // According to the problem statement, the input file with operators is always valid.
                abort();
        }
    }();
    ifs >> priority;
    associativity = [&]() {
        char tmp;
        switch (ifs >> tmp; std::tolower(tmp)) {
            case 'l':
                return op_associativity::Left;
            case 'r':
                return op_associativity::Right;
            default:
                // Safety:
                // According to the problem statement, the input file with operators is always valid.
                abort();
        }
    }();
}