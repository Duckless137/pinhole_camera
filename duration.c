#include "./include/token_reader.h"
#include "./include/token_parser.h"
#include "libs/safe_strcmp.c"
#include <stdlib.h>
#include <math.h>

#define PARSED_VAR_NULL {NULL, -1.0, NULL};

size_t f_stop_table[] = {16, 22, 32, 44, 64, 88, 128, 176, 256, 352, 512, 0};

// Find number closest to focal number (`fnum`)
size_t fnum_lookup(double fnum) {
    size_t i;
    for (i = 0; f_stop_table[i] != 0; i++)
    {
        if (fnum < f_stop_table[i])
        {
            double cmp_less = abs(f_stop_table[i] - fnum);
            double cmp_greater = abs(f_stop_table[i - 1] - fnum);
            if (cmp_less < cmp_greater)
            {
                return i;
            } else {
                return i - 1;
            }
        }
    }
    return 10;
}

void putmchar(char *str)
{
    for (size_t i = 0; str[i] != '\0'; i++)
    {
        putchar(str[i]);
    }
    return;
}

static void print_header(size_t num)
{
    if (num < 10)
    {
        putmchar("--");
        putchar((char) (num + 48));
        putmchar("--");
    } else {
        printf("-%2lu--", num);
    }
    return;
}

void neat_print_fstop(size_t steps)
{
    putchar('+');
    for (size_t i = 0; f_stop_table[i] != 0; i++)
    {
        print_header(i);
        putchar('+');
    }
    putchar('\n');
    putchar('|');
    for (size_t i = 0; f_stop_table[i] != 0; i++)
    {
        printf(" %3lu |", f_stop_table[i]);
    }
    putchar('\n');
    putchar('+');
    for (size_t i = 0; f_stop_table[i] != 0; i++)
    {
        putmchar("-----");
        putchar('+');
    }
    putchar('\n');
    for (size_t i = 0; i < steps; i++)
    {
        putmchar("~~~+~~");
    }
    putmchar("~/^^^\\\n");
}

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

    long double focal_length;
    char *focal_length_unit;

    long double pix_span;
    char *pix_span_unit;

    long double pix_per_unit;
    // Incredible naming conventions
    char *pix_per_unit_unit;

    parse_token_stream(
        &token_stream, 3, 
        "FOCAL_LEN", &focal_length, &focal_length_unit,
        "PIX_SPAN", &pix_span, &pix_span_unit,
        "PIX_PER_UNIT", &pix_per_unit, &pix_per_unit_unit 
    );

    if (safe_strcmp(&pix_per_unit_unit, &pix_span_unit))
    {
        fputs("Fields PIX_SPAN & PIX_PER_UNIT must have the same unit.\n", stderr);
        destroy_token_stream(&token_stream);
        return 1;
    }
    
    // Calculate pinhole diameter & print instructions
    long double pinhole_diam = pix_span / pix_per_unit;
    puts("--Getting pinhole diameter--");
    printf("Pinhole diameter in \"%s\": width / resolution\n", focal_length_unit);
    printf("Pinhole diameter: %.1Lf%s / %.1Lf%s\n", pix_span, pix_span_unit, pix_per_unit, pix_per_unit_unit);
    printf("Pinhole diameter: %Lg%s\n\n", pinhole_diam, focal_length_unit);
    
    // Calculate focal number & print instructions
    long double fnum = focal_length / pinhole_diam;
    puts("--Getting focal number--");
    printf("Focal number: focal length / pinhole diameter\n");
    printf("Focal number: %Lg%s / %Lg%s\n", focal_length, focal_length_unit, pinhole_diam, focal_length_unit);
    printf("Focal number: %Lg\n\n", fnum);

    // Calculate effective focal number & print instructions
    size_t steps = fnum_lookup(fnum);
    long double effective_fnum = f_stop_table[steps];
    puts("--Getting effective f-number--");
    neat_print_fstop(steps);
    printf("%Lg is closest to %.5Lg on the f/stop table\n", fnum, effective_fnum);
    printf("Steps: %lu\n\n", steps);

    // Calculate final time & print instructions
    long double x = powl(2.0, (long double) steps);
    long double T = 0.1 * x;
    puts("--Getting time--");
    puts("T = t * (2 ^ x)");
    puts("-t = 1 / ISO");
    puts("-t = 1 / 10");
    puts("-t = 0.1s");
    printf("T = 0.1s * (2 ^ %lu)\n", steps);
    printf("T = 0.1s * %Lg\n\n", x);
    puts("---FINAL TIME---");
    printf("T_%.0Lf = %Lgs\n", effective_fnum, T);

    // Wrap up code and prevent memory leaks
    destroy_token_stream(&token_stream);    
    return 0;
}