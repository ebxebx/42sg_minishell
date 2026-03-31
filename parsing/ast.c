/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ka-tan <ka-tan@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 17:53:44 by zchoo             #+#    #+#             */
/*   Updated: 2026/03/31 15:55:03 by ka-tan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "../libft/libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to create a new AST node
t_ast	*create_node(char *value)
{
    t_ast	*new_node;

    new_node = (t_ast *)malloc(sizeof(t_ast));
    if (!new_node)
        return (NULL);
    new_node->value = value ? strdup(value) : strdup("");
    new_node->left = NULL;
    new_node->right = NULL;
    return (new_node);
}

// Function to free the AST
void	free_ast(t_ast *node)
{
    if (node)
    {
        free_ast(node->left);
        free_ast(node->right);
        free(node->value);
        free(node);
    }
}

// Function to check if a token is an operator
int	is_operator(char *token)
{
    if (!token)
        return (0);
    return (strcmp(token, "|") == 0
        || strcmp(token, ">") == 0
        || strcmp(token, "<") == 0
        || strcmp(token, ">>") == 0
        || strcmp(token, "<<") == 0
        || strcmp(token, "||") == 0
        || strcmp(token, "&&") == 0);
}

static int is_redirection(const char *tok)
{
    if (!tok)
        return (0);
    return (strcmp(tok, "<") == 0 || strcmp(tok, "<<") == 0
        || strcmp(tok, ">") == 0 || strcmp(tok, ">>") == 0);
}

/*
1️⃣ Lowest level: command
Handles:
- words → argv (concatenate into single node->value)
- redirections (attach file nodes to left for input and right for output)
This function consumes tokens by advancing *tokens to the next unconsumed token.
*/
t_ast	*parse_cmd(t_token **tokens)
{
    t_token	*p;
    t_ast	*ast = NULL;
    char	*cmd_buf = NULL;
    size_t	buf_len = 0;

    if (!tokens || !*tokens)
        return (NULL);
    p = *tokens;

    // Handle leading redirections before command
    while (p && is_redirection(p->value))
    {
        t_token *file = p->next;
        if (!file)
            break; // malformed, stop processing
        if (!ast)
            ast = create_node("");
        if (strcmp(p->value, "<") == 0 || strcmp(p->value, "<<") == 0)
        {
            // input redirection -> attach to left
            if (ast->left)
                free_ast(ast->left);
            ast->left = create_node(file->value);
        }
        else
        {
            // output redirection -> attach to right
            if (ast->right)
                free_ast(ast->right);
            ast->right = create_node(file->value);
        }
        p = file->next;
    }

    // Collect consecutive words into a single command string
    while (p && !is_operator(p->value) && strcmp(p->value, "(") != 0 && !is_redirection(p->value))
    {
        size_t vlen = strlen(p->value);
        char *new_buf = realloc(cmd_buf, buf_len + vlen + 2); // +1 for space or NUL
        if (!new_buf)
        {
            free(cmd_buf);
            return (NULL);
        }
        cmd_buf = new_buf;
        if (buf_len == 0)
            memcpy(cmd_buf + buf_len, p->value, vlen);
        else
        {
            cmd_buf[buf_len] = ' ';
            memcpy(cmd_buf + buf_len + 1, p->value, vlen);
            vlen += 1;
        }
        buf_len += vlen;
        cmd_buf[buf_len] = '\0';
        p = p->next;
    }

    if (cmd_buf)
    {
        if (!ast)
            ast = create_node(cmd_buf);
        else
        {
            free(ast->value);
            ast->value = strdup(cmd_buf);
        }
        free(cmd_buf);
    }
    // Handle trailing redirections after command
    while (p && is_redirection(p->value))
    {
        t_token *file = p->next;
        if (!file)
            break; // malformed
        if (!ast)
            ast = create_node("");
        if (strcmp(p->value, "<") == 0 || strcmp(p->value, "<<") == 0)
        {
            if (ast->left)
                free_ast(ast->left);
            ast->left = create_node(file->value);
        }
        else
        {
            if (ast->right)
                free_ast(ast->right);
            ast->right = create_node(file->value);
        }
        p = file->next;
    }
    *tokens = p;
    return (ast);
}

t_ast *parse_pipeline(t_token *tokens)
{
    t_token *cur = tokens;
    t_ast *left = parse_cmd(&cur);
    if (!left)
        return (NULL);

    while (cur && strcmp(cur->value, "|") == 0)
    {
        // consume '|'
        cur = cur->next;
        // parse right-hand command (advances cur)
        t_ast *right = parse_cmd(&cur);
        t_ast *node = create_node("|");
        node->left = left;
        node->right = right;
        left = node;
    }
    return (left);
}

/* Recursive Function to Parse Tokens into AST */
t_ast	*parse_tokens_to_ast(t_token *tokens)
{
    // TODO: add logical-operator and parenthesis parsing with proper precedence.
    // For now, build pipelines (left-associative). Extend precedence handling later.
    if (!tokens)
        return (NULL);
    return (parse_pipeline(tokens));
}

/* Print AST (for debugging purposes) */
void	print_ast(t_ast *node, int depth)
{
    if (node)
    {
        print_ast(node->right, depth + 1);
        for (int i = 0; i < depth; i++)
            printf("    ");
        printf("%s\n", node->value);
        print_ast(node->left, depth + 1);
    }
}

/* Main function for testing */
#ifdef TEST_AST_MAIN
int	main(void)
{
    const char	*input = "echo 'Hello World' | grep Hello > output.txt | wc -l";
    // const char	*input = "(echo 'Hello World' | grep Hello > output.txt) && (cat input.txt | sort | uniq)";
    t_token		*tokens;
    t_ast		*ast;

    tokens = parse_token(input);
    ast = parse_tokens_to_ast(tokens);
	printf("AST:\n");
    print_ast(ast, 0);
    free_ast(ast);
    return (0);
}
#endif

// Example usage
/* int main(void) {
	t_ast *root = create_node("root");
	root->left = create_node("left");
	root->right = create_node("right");
	free_ast(root);
	return (0);
} */
