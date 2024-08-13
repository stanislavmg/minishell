#ifndef MINISHELL_H
#define MINISHELL_H
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// typedef enum e_type
// {
//     PIPE,
//     AND,
//     OR,
//     SEMICOLON,
//     FILE
// } s_type;

typedef struct s_cmd
{
    char *path;
    char **args;
    int type_out; // O_CREATE | O_TRUNC | O_APPEND etc
    int file_out;
    int file_in; // F_STDOUT || F_STDIN
} t_cmd;

typedef struct s_pars
{
    char *str;
    char *new_str; // string without $variables
    char type_qm;  // " or '
    struct s_pars *next;
} t_pars;



#endif