#ifndef LIB_TOKEN_PARSER_H
#define LIB_TOKEN_PARSER_H
#include "./token_reader.h"

/* This complex function turns tokens into actual,
 * usable data. 
 * 
 * This will take a while to explain, so I'll
 * use DoxyGen notation to keep things organized.
 * 
 * @breif Parses the token stream into variables.
 * @param token_stream The token stream
 * @param argc The amount of fields you want to parse.
 * 
 * For each field you want parsed, you 
 * must add the following arguments:
 * 1. Idenifier (char *)
 * 2. Value (long double *)
 * 3. Unit (char **)
 * 
 * The identifier is the name of the field.
 * For example, if you find a field named "TEST",
 * then it will find the inentifer argument named
 * "TEST" and put the value into the `Value` arg
 * after it and the unit into the `Unit` arg.
 * If you do not need the unit, it can be set to
 * NULL and it will be ignored. Values must be handled
 * though.
 * 
 * @example
 * 
 * ---pinhole.conf---
 * PARAM_1 = 5in
 * PARAM_2 = 7cm
 * 
 * ---main.c---
 * #include <stdio.h>
 * #include "token_reader.h"
 * #include "token_parser.h"
 * 
 * int main()
 * {
 *   FILE *fptr = fopen("./pinhole.conf", "r");
 *   
 *   TokenStream token_stream;
 *   get_token_stream(&token_stream, config_file);
 * 
 *   long double value_1;
 *   char *value_1_unit;
 *   
 *   long double value_2;
 *   parse_token_stream(
 *      &token_stream, 2, 
 *      "PARAM_1", &value_1, &value_1_unit,
 *      "PARAM_2", &value_2, NULL
 *   );
 * 
 *   // Value 1: 5in
 *   printf("Value 1: %Lf%s", value_1, value_1_unit);
 *   
 *   // Value 2: 7
 *   printf("Value 2: %Lf", value_2);
 *   
 *   destroy_token_stream(&token_stream);
 *   return 1;
 * }
 */
void parse_token_stream(TokenStream *token_stream, const size_t argc, ...);
#endif