#ifndef HOMEWORK_1_EXPRESSION_PROCESSING_PARSER_H_INCLUDED
#define HOMEWORK_1_EXPRESSION_PROCESSING_PARSER_H_INCLUDED

#include <string>
#include "tokenizer.h"
#include "operations.h"
#include "scoped_ptr.h"

class parser final {
public:
    parser(const std::string &input, const operations &ops);

public: // Operations
    double eval(double min_priority = 1);

    double eval_atom();

    double eval_op(const op_descriptor &op_desc, double lhs, double rhs);

private:
    const operations &m_ops;
    tokenizer m_tokenizer;
};

#endif //HOMEWORK_1_EXPRESSION_PROCESSING_PARSER_H_INCLUDED
