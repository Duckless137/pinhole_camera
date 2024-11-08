#include "../include/token_reader.h"
#include "../include/token_parser.h"
#include "../libs/safe_strcmp.c"
#include <stdlib.h>
#include <math.h>


// I organized a lot of this code
// into seperate functions, so putting
// varaiables into global scope makes
// a lot of sense here.
long double focal_length;
char *focal_length_unit;

long double film_dimension;
char *film_dimension_unit;

long double object_dimension;
char *object_dimension_unit;

// Function symbols to please
// the compiler
void step_1();
void step_2();
void step_3();

int main(int argc, char **argv)
{
    FILE *config_file;
    if (argc == 1)
    {
        config_file = fopen("./pinhole.conf", "r");
        if (config_file == NULL)
        {
            fprintf(stderr, "Cannot open file \"pinhole.conf\". \nPlease make a pinhole.conf file or specify a path using \"%s <path>\".\n", argv[0]);
            return 1;
        }
        
    } else {
        config_file = fopen(argv[1], "r");
        if (config_file == NULL)
        {
            fprintf(stderr, "Cannot open file \"%s\". \nPlease ensure that the file path exists and has read permissions.\n", argv[1]);
            return 1;
        }
    }
     
    TokenStream token_stream; 
    get_token_stream(&token_stream, config_file);    


    parse_token_stream(
        &token_stream, 3, 
        "FOCAL_LEN", &focal_length, &focal_length_unit,
        "FILM_DIMENSION", &film_dimension, &film_dimension_unit,
        "OBJECT_DIMENSION", &object_dimension, &object_dimension_unit
    );



    // This ensures that all three have the same unit via the transative property.
    if (safe_strcmp(&focal_length_unit, &film_dimension_unit) != 0 || safe_strcmp(&focal_length_unit, &object_dimension_unit) != 0)
    {
        fputs("All fields must have the same unit.\n", stderr);
        destroy_token_stream(&token_stream);    
        return 1;
    }
    
    // Begin calculations
    puts("film dimension   object dimension");
    puts("-------------- = ----------------");
    puts(" focal length       distance\n\n");

    step_1();
    step_2();
    step_3();


    long double ratio = focal_length / film_dimension;
    long double res = ratio * object_dimension;

    printf("%Lg%s = distance\n", res, object_dimension_unit);

    // Wrap up code and prevent memory leaks
    destroy_token_stream(&token_stream);    
    return 0;
}


void print_in_center(size_t line_len, char *value, char *unit, size_t total_len)
{
    long double white_space = line_len - total_len;
    
    for (size_t i = 0; i < floor(white_space / 2); i++)
    {
        putchar(' ');
    }
    
    fputs(value, stdout); 
    fputs(unit, stdout); 

    for (size_t i = 0; i < ceil(white_space / 2); i++)
    {
        putchar(' ');
    }
}

void print_fraction_lines();
void putc_x_times(char character, unsigned amount)
{
    for (size_t i = 0; i < amount; i++)
    {
        putchar(character);
    }
    
}

// These need to be used in multiple steps.
size_t line_1_len;
size_t line_2_len = 8;

size_t focal_length_plus_unit_len;
size_t film_dimension_plus_unit_len;
size_t object_dimension_plus_unit_len;

char focal_length_as_str[30];
char film_dimension_as_str[30];
char object_dimension_as_str[30];

// Display all the values in fraction form
void step_1()
{

    sprintf(focal_length_as_str, "%Lg", focal_length);
    sprintf(film_dimension_as_str, "%Lg", film_dimension);        

    // I'm not sure if these variable names are good or terrible.
    focal_length_plus_unit_len = strlen(focal_length_as_str) + strlen(focal_length_unit);
    film_dimension_plus_unit_len = strlen(film_dimension_as_str) + strlen(film_dimension_unit);

    if (focal_length_plus_unit_len > film_dimension_plus_unit_len)
    {
        line_1_len = focal_length_plus_unit_len;
        print_in_center(
            line_1_len, 
            film_dimension_as_str, 
            film_dimension_unit, 
            film_dimension_plus_unit_len
        );
    } else {
        line_1_len = film_dimension_plus_unit_len;
        fputs(film_dimension_as_str, stdout); 
        fputs(film_dimension_unit, stdout); 
    }
    
    putc_x_times(' ', 3);
    
    sprintf(object_dimension_as_str, "%Lg", object_dimension);        
    object_dimension_plus_unit_len = strlen(object_dimension_as_str) + strlen(object_dimension_unit);

    if (object_dimension_plus_unit_len > line_2_len)
    {
        line_2_len = object_dimension_plus_unit_len;
        fputs(object_dimension_as_str, stdout);
        puts(object_dimension_unit); 
    } else {
        print_in_center(
            line_2_len, 
            object_dimension_as_str, 
            object_dimension_unit, 
            object_dimension_plus_unit_len
        );
        putchar('\n');
    }

    print_fraction_lines();

    print_in_center(
        line_1_len, 
        focal_length_as_str, 
        focal_length_unit, 
        focal_length_plus_unit_len
    );

    putc_x_times(' ', 3);

    print_in_center(
        line_2_len, 
        "distance", 
        "", 
        8
    );
    puts("\n\n");
}

// Recipricate fractions
void step_2()
{
    print_in_center(
        line_1_len, 
        focal_length_as_str, 
        focal_length_unit, 
        focal_length_plus_unit_len
    );

    putc_x_times(' ', 3);

    print_in_center(
        line_2_len, 
        "distance", 
        "", 
        8
    );

    putchar('\n');
    print_fraction_lines();

    print_in_center(
        line_1_len, 
        film_dimension_as_str, 
        film_dimension_unit, 
        film_dimension_plus_unit_len
    );

    putc_x_times(' ', 3);

    print_in_center(
        line_2_len, 
        object_dimension_as_str, 
        object_dimension_unit, 
        object_dimension_plus_unit_len
    );
    puts("\n\n");
}

// Multiply
void step_3()
{
    putc_x_times(' ', object_dimension_plus_unit_len + 3);
    print_in_center(
        line_1_len, 
        focal_length_as_str, 
        focal_length_unit, 
        focal_length_plus_unit_len
    );

    putchar('\n');

    fputs(object_dimension_as_str, stdout); 
    fputs(object_dimension_unit, stdout); 
    fputs(" * ", stdout); 

    putc_x_times('-', line_1_len);

    puts(" = distance");

    putc_x_times(' ', object_dimension_plus_unit_len + 3);
    print_in_center(
        line_1_len, 
        film_dimension_as_str, 
        film_dimension_unit, 
        film_dimension_plus_unit_len
    );

    puts("\n\n");
}

void print_fraction_lines()
{
    putc_x_times('-', line_1_len);
    fputs(" = ", stdout);
    putc_x_times('-', line_2_len);    
    putchar('\n');
}