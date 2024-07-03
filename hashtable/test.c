#include "hashtable.h"


int	main(int ar, char **av, char **env)
{
	int i = 0;
	char **t;
	t_table *table;
	t_item  *node;

	table = create_table(CAPACITY);
	if (!table)
		return (1);
	while (env[i])
	{
		t = ft_split(env[i], '=');
		add_item(table, t[0], t[1]);
		printf("Key %s\nValue %s\nhas been added\n\n", t[0], t[1]);
		if (t[0])
			free(t[0]);
		if (t[1])
			free(t[1]);
		free(t);
		i++;
	}
	add_item(table, "STANISLAV", "01.08.1998");
	node = search_item(table, "STANISLAV");
	printf("node->key = %s\nnode->value = %s\n", node->key, node->value);
	printf("Start delete table\n");
	delete_table(table);
	//free(table);
	return (0);
}