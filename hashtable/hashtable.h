#ifndef HASHTABLE_H
# define HASHTABLE_H
# include "../libft/libft.h"
# include <stdlib.h>
# include <stdio.h>
# define CAPACITY 2000
# define ERR_MEM 112

typedef struct s_item
{
    char    *key;
    char    *value;
    struct  s_item *next;
} t_item;

typedef struct s_table
{
    t_item  **items;
    int     size;
    int     item_num;
} t_table;


int	    delete_bucket(t_item **lst);
int 	add_item(t_table *table, char *key, char *value);
int	    delete_item(t_item **lst, t_item *node);
t_item  *search_item(t_table *table, char *key);
int 	handle_collision(t_item **lst, t_item *new);
int     delete_table(t_table *table);
t_table *create_table(int size);
size_t  hash_function(char* str);

#endif
