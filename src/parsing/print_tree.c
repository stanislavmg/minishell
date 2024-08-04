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