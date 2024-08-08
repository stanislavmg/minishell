#include "parser.h"

void	print_tokens(t_list *tokens)
{
	int i = 1;

	while (tokens)
	{
		printf("#%d token_word: %s\ntype: %s\n\n", i, 
				((t_token*)tokens->data)->word,
				get_type(((t_token*)tokens->data)->type));
		i++;
		tokens = tokens->next;
	}
}

const char *get_type(int type)
{
	if (END == type)
		return ("END");
	else if (IO_FILE == type)
		return ("IO_FILE");
	else if (REDIRECT == type)
		return ("REDIRECT");
	else if (STRING == type)
		return ("STRING");
	else if (COMMAND == type)
		return ("COMMAND");
	else if(OR == type)
		return ("OR");
	else if (SEMICOLON == type)
		return ("SEMICOLON");
	else if (PIPE == type)
		return ("PIPE");
	else if (AND == type)
		return ("AND");
	else if (VARIABLE == type)
		return ("VARIABLE");
	else if (HERE_DOC == type)
		return ("HERE_DOC");
	else if (INPUT_TRUNC == type)
		return ("INPUT_TRUNC");
	else if (OUTPUT_ADD == type)
		return ("OUTPUT_ADD");
	else if (OUTPUT_TRUNC == type)
		return ("OUTPUT_TRUNC");
	else if (OPEN_BRACKET == type)
		return ("OPEN_BRACKET");
	else if (CLOSED_BRACKET == type)
		return ("CLOSED_BRACKET");
	else if (ASSIGNMENT == type)
		return ("ASSIGNMENT");
	return (NULL);
}

#include "parser.h"

static void printSpaces(int count) {
    for (int i = 0; i < count; i++) {
        putchar(' ');
    }
}

static void printTreeHelper(t_ast* root, int space, int level) {
    if (root == NULL) {
        return;
    }

    // Increase distance between levels
    space += 5;

    // Process right child first
	if (root->type != COMMAND && root->type != VARIABLE && root->type != IO_FILE)
    	printTreeHelper((t_ast *)root->right, space, level + 1);

    // Print current node after space count
    printf("\n");
    printSpaces(space - 5);
    printf("%s", get_type(root->type));
	if (root->type == COMMAND)
		printf(" = %s", ((t_exec_cmd *)root)->argv[0]);
	else if (root->type == VARIABLE)
		printf(" = key=%s value=%s", ((t_var *)root)->key, ((t_var *)root)->value);
    else if (root->type == IO_FILE)
        printf(" = fname=%s", ((t_redir *)root)->fname);
	printf("\n");    
    // Process left child
	if (root->type != COMMAND && root->type != VARIABLE && root->type != IO_FILE)
    	printTreeHelper((t_ast *)root->left, space, level + 1);
}


void print_tree(t_ast* root) {
    printTreeHelper(root, 0, 0);
}