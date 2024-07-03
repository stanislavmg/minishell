#include "hashtable.h"

size_t hash_function(char* str)
{
    size_t	h;
	int		j;

	h = 0;
	j = -1;
	while (str[++j])
		h += str[j];
    return (h % CAPACITY);
}

t_table *create_table(int size)
{
	t_table	*res;

	res = (t_table *)malloc(sizeof(t_table));
	if (!res)
		return (NULL);
	res->size = size;
	res->item_num = 0;
	res->items = (t_item **)malloc(sizeof(t_table *) * size);
	if (!res->items)
	{
		free(res);
		return (NULL);
	}
	return (res);
}

int delete_table(t_table *table)
{
	int i;
	int count;

	i = 0;
	count = 0;
	while (i < table->size)
	{
		if (table->items[i])
			count += delete_bucket(&table->items[i]);
		i++;
	}
	if (count == table->item_num)
	{
		free(table->items);
		free(table);
		return (0);
	}
	return (1);
}

int	handle_collision(t_item **lst, t_item *new)
{
	t_item	*t;
	t_item	*prev;

	t = *lst;
	prev = t;
	if (!new)
		return (1);
	while (t)
	{
		if (!ft_strncmp(t->key, new->key, ft_strlen(new->key)))
		{
			new->next = t->next;
			free(t);
			prev->next = new;
			return (0);
		}
		prev = t;
		t = t->next;
	}
	prev->next = new;
	return (0);
}
