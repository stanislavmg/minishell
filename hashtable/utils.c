#include "hashtable.h"

int	delete_bucket(t_item **lst)
{
	t_item *t;
	int		count;

	count = 0;
	t = *lst;
	while (*lst)
	{
		t = (*lst)->next;
		free((*lst)->key);
		free((*lst)->value);
		free(*lst);
		*lst = t;
		count++;
	}
	*lst = NULL;
	return (count);
}

int	add_item(t_table *table, char *key, char *value)
{
	size_t	index;
	t_item	*new;
	
	if (!key || !value)
		return (1);
	index = hash_function(key);
	printf("Hash index = %ld\n", index);
	new = (t_item *)ft_calloc(1, sizeof(t_item));
	if (!new)
		return (ERR_MEM);
	new->key = ft_strdup(key);
	if (!new->key)
	{
		free(new);
		return (ERR_MEM);
	}
	new->value = ft_strdup(value);
	if (!new->value)
	{
		free(new->key);
		free(new);
		return (ERR_MEM);
	}
	new->next = NULL;
	if (table->items[index])
		handle_collision(&table->items[index], new);
	else
		table->items[index] = new;
	return (0);
}

int	delete_item(t_item **lst, t_item *node)
{
	t_item *t;
	t_item *prev;

	t = *lst;
	prev = t;
	if (!node || !lst || !*lst)
		return (0);
	while (t && t != node)
	{
		prev = t;
		t = t->next;
	}
	if (t)
	{
		free(node->key);
		free(node->value);
		free(node);
		prev->next = NULL;
		return (0);
	}
	return (1);
}

t_item *search_item(t_table *table, char *key)
{
	int	index;
	t_item *t;

	index = hash_function(key);
	if (!table || !key)
		return (NULL);
	t = table->items[index];
	if (t)
	{
		while (t && ft_strncmp(key, t->key, ft_strlen(key)))
			t = t->next;
		return (t);
	}
	return (NULL);
}