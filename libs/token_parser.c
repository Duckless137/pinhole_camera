#include "../include/token_reader.h"
#include "../include/token_parser.h"
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
// Did this end up needing its own file?
// No, but I don't feel like refactoring the
// Makefile. 

typedef struct
{
    char *name;
    long double value;
    char *unit;
} ParsedVariable;

void parse_token_stream(TokenStream *token_stream, size_t argc, ...)
{
    ParsedVariable *variable_array;
    size_t var_arr_idx = 0;
    Token token;
    for (size_t i = 0; i < token_stream->len; i++)
    {   
        token = token_stream->tokens[i];
        if (token.typ == TOKEN_IDENT)
        {
            long double value;
            // There's no atolf() function :(
            // Have to use sscanf()
            sscanf(token_stream->tokens[i + 2].val, "%Lf", &value);
            ParsedVariable var = {
                token.val,
                value,
                NULL
            };      
            if (token_stream->tokens[i + 3].typ == TOKEN_UNIT)
            {
                var.unit = token_stream->tokens[i + 3].val;
            }
                  

            if (var_arr_idx == 0)
            {
                variable_array = malloc(sizeof(ParsedVariable));
            } else {
                variable_array = realloc(variable_array, sizeof(ParsedVariable) * (var_arr_idx + 1));
            }
            

            variable_array[var_arr_idx++] = var;
            i += 3;
        } else 
        // Skip endl tokens
        {
            continue;
        }        
    }

    


    va_list args;
    va_start(args, argc);

    char *ident_hash[argc];
    long double *variable_hash[argc];
    char **unit_hash[argc];

    // Load arguments into hash tables
    for (size_t i = 0; i < argc; i++)
    {
        ident_hash[i] = va_arg(args, char *);
        variable_hash[i] = va_arg(args, long double *);
        unit_hash[i] = va_arg(args, char **);
    }


    va_end(args);

    // Set variables provided by args
    for (size_t i = 0; i < var_arr_idx; i++)
    {
        for (size_t j = 0; j < argc; j++)
        {
            if (strcmp(variable_array[i].name, ident_hash[j]) == 0)
            {
                if (variable_hash[j] == NULL)
                {
                    fprintf(stderr, "Error: Multiple defenitions of \"%s\".\n", ident_hash[j]);
                    free(variable_array);
                    exit(1);
                }
                
                *variable_hash[j] = variable_array[i].value;

                // Ignore if NULL provided
                if (unit_hash[j] != NULL)
                {
                    *unit_hash[j] = variable_array[i].unit;
                }
                
                variable_hash[j] = NULL;
                break;
            }
            
        }
    }
    
    // Prevents an invalid free
    if (var_arr_idx > 0)
    {
        free(variable_array);
    }

    // List all missing fields
    char is_missing_vars = 1;
    for (size_t i = 0; i < argc; i++)
    {
        if (variable_hash[i] != NULL)
        {
            is_missing_vars = 0;
            fprintf(stderr, "Missing field \"%s\"\n", ident_hash[i]);
        }
        
    }    

    if (is_missing_vars == 0)
    {
        exit(1);
    }
    return;
}