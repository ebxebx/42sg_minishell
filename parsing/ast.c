/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 17:53:44 by zchoo             #+#    #+#             */
/*   Updated: 2026/04/04 16:45:37 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void	free_redirs(t_redir *redir)
{
	t_redir	*next;

	while (redir)
	{
		next = redir->next;
		free(redir->file);
		free(redir);
		redir = next;
	}
}

static int	append_redir(t_ast *ast, t_token_type type, const char *file)
{
	t_redir	*new_redir;
	t_redir	*cur;

	if (!ast || !file)
		return (1);
	new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
		return (1);
	new_redir->type = type;
	new_redir->file = ft_strdup(file);
	new_redir->next = NULL;
	if (!new_redir->file)
		return (free(new_redir), 1);
	if (!ast->redirs)
	{
		ast->redirs = new_redir;
		return (0);
	}
	cur = ast->redirs;
	while (cur->next)
		cur = cur->next;
	cur->next = new_redir;
	return (0);
}

// Function to create a new AST node
t_ast	*create_node(char *value)
{
	t_ast	*new_node;

	new_node = (t_ast *)malloc(sizeof(t_ast));
	if (!new_node)
		return (NULL);
	if (value)
		value = ft_strdup(value);
	else
		value = ft_strdup("");
	new_node->value = value;
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->redirs = NULL;
	return (new_node);
}

// Function to free the AST
void	free_ast(t_ast *node)
{
	if (node)
	{
		free_ast(node->left);
		free_ast(node->right);
		free_redirs(node->redirs);
		free(node->value);
		free(node);
	}
}

// Function to check if a token is an operator
int	is_operator(char *token)
{
	if (!token)
		return (0);
	return (ft_strcmp(token, "|") == 0 || ft_strcmp(token, ">") == 0
		|| ft_strcmp(token, "<") == 0 || ft_strcmp(token, ">>") == 0
		|| ft_strcmp(token, "<<") == 0 || ft_strcmp(token, "||") == 0
		|| ft_strcmp(token, "&&") == 0);
}

static int	is_redirection_type(t_token_type type)
{
	return (type == TOK_RDIR_IN || type == TOK_RDIR_OUT
		|| type == TOK_RDIR_HEREDOC || type == TOK_RDIR_APPEND);
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
	t_ast	*ast;
	char	*cmd_buf;
	size_t	buf_len;
	size_t	vlen;
	char	*new_buf;

	cmd_buf = NULL;
	buf_len = 0;
	if (!tokens || !*tokens)
		return (NULL);
	p = *tokens;
	ast = create_node("");
	if (!ast)
		return (NULL);
	while (p && ft_strcmp(p->value, "|") != 0 
		/*&& ft_strcmp(p->value, "(") != 0
		&& ft_strcmp(p->value, ")") != 0 
		&& ft_strcmp(p->value, "&&") != 0
		&& ft_strcmp(p->value, "||") != 0*/)
	{
		// Ether word or redirection
		if (is_redirection_type(p->type))
		{
			if (!p->next || p->next->type != TOK_WORD || append_redir(ast,
					p->type, p->next->value))
			{
				free(cmd_buf);
				free_ast(ast);
				return (NULL);
			}
			p = p->next->next;
			continue ;
		}
		vlen = ft_strlen(p->value);
		// TODO realloc need to change or create own function
		new_buf = realloc(cmd_buf, buf_len + vlen + 2);
		if (!new_buf)
		{
			free(cmd_buf);
			free_ast(ast);
			return (NULL);
		}
		cmd_buf = new_buf;
		if (buf_len == 0)
			ft_memcpy(cmd_buf + buf_len, p->value, vlen);
		else
		{
			cmd_buf[buf_len] = ' ';
			ft_memcpy(cmd_buf + buf_len + 1, p->value, vlen);
			vlen += 1;
		}
		buf_len += vlen;
		cmd_buf[buf_len] = '\0';
		p = p->next;
	}
	if (cmd_buf)
	{
		free(ast->value);
		ast->value = ft_strdup(cmd_buf);
		free(cmd_buf);
		if (!ast->value)
			return (free_ast(ast), NULL);
	}
	*tokens = p;
	return (ast);
}

t_ast	*parse_pipeline(t_token *tokens)
{
	t_token	*cur;
	t_ast	*left;
	t_ast	*right;
	t_ast	*node;

	cur = tokens;
	left = parse_cmd(&cur);
	if (!left)
		return (NULL);
	while (cur && ft_strcmp(cur->value, "|") == 0)
	{
		// consume '|'
		cur = cur->next;
		// parse right-hand command (advances cur)
		right = parse_cmd(&cur);
		node = create_node("|");
		node->left = left;
		node->right = right;
		left = node;
	}
	return (left);
}

/* Recursive Function to Parse Tokens into AST */
// TODO: add logical-operator and parenthesis parsing with proper precedence.
// For now,	build pipelines (left-associative). Extend precedence handling later.
t_ast	*parse_tokens_to_ast(t_token *tokens)
{
	if (!tokens)
		return (NULL);
	return (parse_pipeline(tokens));
}

/* Print AST (for debugging purposes) */
void	print_ast(t_ast *node, int depth)
{
	t_redir	*redir;
	int		i;

	if (!node)
		return ;
	if (depth == 0)
		ft_printf("AST:\n");
	print_ast(node->right, depth + 1);
	i = 0;
	while (i++ < depth)
		ft_printf("    ");
	ft_printf("%s", node->value);
	redir = node->redirs;
	if (redir)
		ft_printf(" [");
	while (redir)
	{
		if (redir->type == TOK_RDIR_IN)
			ft_printf("< %s", redir->file);
		else if (redir->type == TOK_RDIR_OUT)
			ft_printf("> %s", redir->file);
		else if (redir->type == TOK_RDIR_APPEND)
			ft_printf(">> %s", redir->file);
		else if (redir->type == TOK_RDIR_HEREDOC)
			ft_printf("<< %s", redir->file);
		if (redir->next)
			ft_printf(", ");
		redir = redir->next;
	}
	if (node->redirs)
		ft_printf("]");
	ft_printf("\n");
	print_ast(node->left, depth + 1);
}

/* Main function for testing */
#ifdef TEST_AST_MAIN
int	main(void)
{
	const char	*input = "echo 'Hello World' | grep Hello > output.txt | wc -l";
	t_token		*tokens;
	t_ast		*ast;

	// const char	*input = "(echo 'Hello World' | grep Hello > output.txt)
			&& (cat input.txt | sort | uniq)";
	tokens = parse_token(input);
	ast = parse_tokens_to_ast(tokens);
	ft_printf("AST:\n");
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
