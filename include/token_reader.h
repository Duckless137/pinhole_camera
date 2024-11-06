#ifndef LIB_TOKEN_READER_H
#define LIB_TOKEN_READER_H
#include <stdio.h>

// All token types
typedef enum
{
    TOKEN_IDENT,
    TOKEN_EQ,
    TOKEN_VAL,
    TOKEN_UNIT,
    TOKEN_ENDL
} TokenTypes;

// The actual token. Contains the tokens
// type, a heap allocated string value,
// and the length of said value.
typedef struct
{    
    TokenTypes typ;
    unsigned long val_len;
    char *val;
} Token;

// Dynamically allocated array of tokens
// with metadata at the front, such as its
// origin config file and the vector length.
typedef struct
{
    FILE *fptr;
    unsigned long len;
    Token *tokens;
} TokenStream;

// Parses tokens from a config file into a token stream.
extern void get_token_stream(TokenStream *restrict __stream, FILE *restrict fptr);

// Frees the token stream, preventing memory leaks.
extern void destroy_token_stream(TokenStream *stream);
#endif