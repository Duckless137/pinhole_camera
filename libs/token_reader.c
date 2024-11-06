#include "../include/token_reader.h"
#include <stdio.h>
#include <stdlib.h>

/* These act as a kind of "focus"
 * for the scope. This does mean
 * that only one TokenStream can
 * be created at a time though, but
 * luckily my system doesn't use
 * multithreading.
 */
static Token *token;
static size_t line;
static TokenStream *stream;

// Increments the side of the tokenstream.
static void resize_token_stream(TokenStream *stream)
{
    stream->tokens = realloc(stream->tokens, sizeof(Token) * (stream->len + 1));
    stream->len++;
    return;
}

// This will DESTROY! the token stream,
// preventing memory leaks
void destroy_token_stream(TokenStream *stream)
{
    Token *tokens = stream->tokens;
    fclose(stream->fptr);
    for (size_t i = 0; i < stream->len; i++)
    {
        char *str_ptr = tokens[i].val;
        free(str_ptr);
    }
    free(tokens);
}

// Edits a token if it's of the same type, or adds a new one if not.
static void change_or_add_token(TokenTypes new_typ, char new_char)
{
    Token *new_token = token;
    if (new_token != NULL && new_token->typ == new_typ)
    {
        char *str = new_token->val;
        size_t len = new_token->val_len++;
        new_token->val = realloc(str, len + 2);
        new_token->val[len] = new_char;
        new_token->val[len + 1] = '\0';
    }
    else
    {
        resize_token_stream(stream);
        unsigned long len = stream->len;
        new_token = token = &stream->tokens[len - 1];
        new_token->typ = new_typ;
        new_token->val_len = 1;
        char *str = new_token->val = malloc(2);
        str[0] = new_char;
        str[1] = '\0';
    }
}

// Gets the last token in the stream. 
// Used for certain runtime checks.
static Token *get_previous_token()
{
    if (stream->len < 2)
    {
        return NULL;
    }

    return &stream->tokens[stream->len - 2];
}

// Neatly prints out an error message and the
// line the error occured on.
static void print_err(char *msg)
{
    fprintf(stderr, "Err on line %lu: \"%s\"\n", line, msg);
    destroy_token_stream(stream);
    exit(1);
}

void get_token_stream(TokenStream *restrict __stream, FILE *restrict fptr)
{
    stream = __stream;

    // Initalize tokenstream
    stream->tokens = malloc(sizeof(TokenStream));
    stream->fptr = fptr;
    stream->len = 0;

    // Some necessary variables
    char buf[1024];
    char character;
    line = 1;
    token = NULL;

    // I chose to use fgets instead of fread
    // since it will automatically detect newline
    // characters
    while (fgets(buf, 1024, fptr))
    {
        // Reads through data
        for (size_t i = 0; (character = buf[i]) != '\0'; i++)
        {
            // If the character is a hashtag,
            // the line is a comment. Panics 
            // if it's not at the start of
            // the line.
            if (character == '#')
            {
                if (token != NULL && token->typ != TOKEN_ENDL)
                {
                    print_err("Unexpected line termination. Please make sure comments are always at the start of the line.");
                }
                break;
            }
            // If the characher is an equals sign,
            // it's a TOKEN_EQ. Runtime checks ensure
            // positioning.
            else if (character == '=')
            {
                if (token == NULL || token->typ != TOKEN_IDENT)
                {
                    print_err("Token EQ must come after an ident");
                }
                change_or_add_token(TOKEN_EQ, character);
            }
            // Numbers(between 47 and 58 in UTF-8)
            // and periods (46 in utf-8) are always 
            // TOKEN_VAL. Once again, runtime checks
            // ensure positioning.
            else if ((character > 47 && character < 58) || character == '.')
            {
                if (token->typ == TOKEN_IDENT || token->typ == TOKEN_ENDL)
                {
                    change_or_add_token(TOKEN_IDENT, character);                                    
                } else {
                    change_or_add_token(TOKEN_VAL, character);  
                    Token *prev_token = get_previous_token();
                    if (prev_token != NULL && prev_token->typ != TOKEN_EQ)
                    {
                        print_err("Token VAL must come after an EQ");
                    }                              
                } 
            // Uppercase characters (between 64 and 91 in UTF-8)
            // and underscores (95 in UTF-8) are always TOKEN_IDENT.
            // Once again, runtime checks make sure the user doesn't
            // do something completely stupid.
            } else if ((character > 64 && character < 91) || character == '_')
            {
                change_or_add_token(TOKEN_IDENT, character);                
                Token *prev_token = get_previous_token();
                if (prev_token != NULL && prev_token->typ != TOKEN_ENDL)
                {
                    printf("Prev token: %i\n", prev_token->typ);
                    if (token->typ == TOKEN_UNIT)
                    {
                        print_err("Token UNIT cannot contain uppercase chars or underscores");
                    } else {
                        print_err("Token IDENT must come after an ENDL");
                    }
                }
            // Lowercase characters (between 96 and 123 in UTF-8)
            // are always TOKEN_UNIT. If you know of any units that
            // use uppercase characters, please let me know and I
            // will promptly ignore it.
            } else if (character > 96 && character < 123)
            {
                change_or_add_token(TOKEN_UNIT, character);
                Token *prev_token = get_previous_token();
                if (prev_token == NULL || prev_token->typ == TOKEN_ENDL || prev_token->typ == TOKEN_IDENT)
                {
                    print_err("Cannot use lowercase letters in IDENT");
                } else if (prev_token->typ == TOKEN_EQ)
                {
                    print_err("Must have a number before a unit");
                }
                
            }
            // Newline characters are newlines - no runtime
            // checks here. There are enough already at this
            // point.
            else if (character == '\n')
            {
                if (token->typ != TOKEN_UNIT && token->typ != TOKEN_VAL)
                {
                    print_err("No value provided.");
                }
                
                change_or_add_token(TOKEN_ENDL, '\n');
            // Whitespace is ignored entirely.
            } else if (character == ' ')
            {
                continue;
            }
            // Exits the program if the user types something
            // totally insane. Cannot use `print_err` function
            // because it has to print out the totally insane 
            // character the user typed.
            else
            {
                fprintf(stderr, "Err on line %lu: Invalid char \"%c\"\n", line, character);
                destroy_token_stream(stream);
                exit(1);
            }
        }
        line++;
    }
    // Adds a TOKEN_ENDL for saftey.
    change_or_add_token(TOKEN_ENDL, '\n');
    return;
}
