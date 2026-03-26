#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct s_ast {
    char *value;
    struct s_ast *left;
    struct s_ast *right;
} t_ast;

// Function to create a new AST node

t_ast *create_node(char *value) {
    t_ast *new_node = (t_ast *)malloc(sizeof(t_ast));
    new_node->value = strdup(value);
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}

// Function to free the AST
void free_ast(t_ast *node) {
    if (node) {
        free_ast(node->left);
        free_ast(node->right);
        free(node->value);
        free(node);
    }
}

// Example usage
int main() {
    t_ast *root = create_node("root");
    root->left = create_node("left");
    root->right = create_node("right");
    free_ast(root);
    return 0;
}
