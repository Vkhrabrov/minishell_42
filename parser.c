/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkhrabro <vkhrabro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 17:44:14 by vadimhrabro       #+#    #+#             */
/*   Updated: 2024/01/28 00:43:18 by vkhrabro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

struct token	*handle_redirections(struct command_node *cmd_node, \
		struct token *current, struct token **last_arg, struct token **tokens)
{
	if (current->next && current->next->type == T_ARG)
	{
		add_redirection(cmd_node, current, current->next->content);
		*last_arg = NULL;
		return (current->next);
	}
	else
	{
		handle_redirection_error(current, tokens);
		return (NULL);
	}
}

t_token	*duplicate_token(const t_token *original)
{
	t_token	*duplicate;

	duplicate = malloc(sizeof(t_token));
	if (!duplicate)
	{
		perror("Memory allocation failed for token duplication");
		exit(EXIT_FAILURE);
	}
	duplicate->content = ft_strdup(original->content);
	duplicate->type = original->type;
	duplicate->next = duplicate->prev;
	duplicate->prev = NULL;
	return (duplicate);
}

int	process_current_token(struct token **current, \
	struct command_node *cmd_node, struct token ***last_arg, \
	struct token **tokens)
{
	if ((*current)->type == T_CMD)
	{
		if (!cmd_node->command)
			cmd_node->command = *current;
	}
	else if ((*current)->type == T_ARG)
	{
		**last_arg = *current;
		*last_arg = &(*current)->next;
	}
	else if ((*current)->type == T_REDIR_IN \
		|| (*current)->type == T_APP_REDIR \
		|| (*current)->type == T_REDIR_OUT)
	{
		*current = handle_redirections(cmd_node, *current, *last_arg, tokens);
		if (!(*current))
			return (1);
	}
	else
		process_other_tokens(*current, cmd_node, last_arg);
	*current = (*current)->next;
	return (0);
}

struct command_node	*parse_command(struct token **tokens)
{
	struct command_node	*cmd_node;
	struct token		*current;
	struct token		**last_arg;

	last_arg = NULL;
	cmd_node = NULL;
	if (!tokens || !(*tokens))
		return (NULL);
	cmd_node = ft_calloc(1, sizeof(struct command_node));
	if (!cmd_node)
	{
		perror("Memory allocation failed for command_node");
		exit(EXIT_FAILURE);
	}
	current = *tokens;
	last_arg = &(cmd_node->args);
	while (current && current->type != T_PIPE)
		if ((process_current_token(&current, cmd_node, &last_arg, tokens)) == 1)
			return (NULL);
	*last_arg = NULL;
	*tokens = current;
	return (cmd_node);
}

struct command_node	*parse_line(struct token *tokens)
{
	struct command_node	*head;
	struct command_node	*prev;
	struct command_node	*current;

	head = NULL;
	prev = NULL;
	while (tokens)
	{
		current = parse_command(&tokens);
		if (!current)
			return (NULL);
		if (!head)
		{
			head = current;
			prev = head;
		}
		else
		{
			prev->next = current;
			prev = current;
		}
		if (!handle_pipe_tokens(&tokens, &current))
			return (NULL);
	}
	return (head);
}
