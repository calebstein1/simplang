#ifndef SIMPLANG_PARSE_H
#define SIMPLANG_PARSE_H

void parse_op(operation *op);
void get_opcode(operation *op, char *tok);
void parse_one_arg(operation *op);
void parse_two_args(operation *op);
void parse_three_args(operation *op);

#endif //SIMPLANG_PARSE_H
