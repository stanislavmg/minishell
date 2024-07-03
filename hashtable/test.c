#include "hashtable.h"


int	main(int ar, char **av, char **env)
{
	int i = 0;
	char **t;
	t_table *table;

	table = create_table(CAPACITY);
	if (!table)
		return (1);
	while (env[i])
	{
		t = ft_split(env[i], '=');
		add_item(table, t[0], t[1]);
		printf("Key %s\nValue %s\n has been added\n\n", t[0], t[1]);
		free(t[0]);
		free(t[1]);
		free(t);
		i++;
	}
	return (0);
}