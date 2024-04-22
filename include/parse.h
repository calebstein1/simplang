#ifndef SIMPLANG_PARSE_H
#define SIMPLANG_PARSE_H

void get_opcode(operation *op, char *tok, char **tok_pos);
void parse_op(operation *op, char **tok_pos);

#endif //SIMPLANG_PARSE_H
