#ifndef MINISHEL_H
#define MINISHEL_H
#include "hashtable.h"
#include "parser.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef struct s_cmd
{
    char **argv;
    t_table *env;
    int pdes[2];
    int type_out; // O_CREATE | O_TRUNC | O_APPEND etc > 
    int file_out;
    int file_in; // F_STDOUT || F_STDIN
} t_cmd;

// this shit will be free when the t_cmd list is ready
typedef struct s_pars
{
    char *str;
    char *new_str; // string without $variables
    char type_qm;  // " or '
    struct s_pars *next;
} t_pars;



#endif