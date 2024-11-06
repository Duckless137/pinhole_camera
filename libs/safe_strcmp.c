#include <string.h>
// Since this function is shared it has its
// own file, but no header file since it's
// so small and doesn't need documentation.

// Strcmp without null derefs
int safe_strcmp(char **string_1, char **string_2)
{
    // Normally you wouldn't want to 
    // set them to blank, but I do
    // this anyway in both calculators
    // so I'm just killing two birds
    // with one stone.
    if (*string_1 == NULL)
    {
        *string_1 = "";
    }
    if (*string_2 == NULL)
    {
        *string_2 = "";
    }
    
    // Why reinvent the wheel?
    return strcmp(*string_1, *string_2);
}