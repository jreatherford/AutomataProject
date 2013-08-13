#include "scanner.h"

#ifndef PARSER_H
#define PARSER_H

/*
dem functions
*/
void printError (char* expected, token* found, bool* errorFree);
bool parse_parse (scanner* scan);
void parse_dec_list (scanner* scan, token* curr_token, bool* errorFree);
void parse_dec (scanner* scan, token* curr_token, bool* errorFree);
void parse_id_list (scanner* scan, token* curr_token, bool* errorFree);
void parse_type(scanner* scan, token* curr_token, bool* errorFree);
void parse_stmt_list (scanner* scan, token* curr_token, bool* errorFree);
void parse_stmt (scanner* scan, token* curr_token, bool* errorFree);
void parse_assign (scanner* scan, token* curr_token, bool* errorFree);
void parse_expression (scanner* scan, token* curr_token, bool* errorFree);
void parse_term (scanner* scan, token* curr_token, bool* errorFree);
void parse_factor (scanner* scan, token* curr_token, bool* errorFree);
void parse_read_write (scanner* scan, token* curr_token, bool* errorFree);
void parse_for (scanner* scan, token* curr_token, bool* errorFree);
void parse_index_exp (scanner* scan, token* curr_token, bool* errorFree);
void parse_body (scanner* scan, token* curr_token, bool* errorFree);

#endif
