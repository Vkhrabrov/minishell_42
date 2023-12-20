/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 17:44:14 by vadimhrabro       #+#    #+#             */
/*   Updated: 2023/12/20 21:06:09 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*	parse_command() 
 *	
 *	while a pipe is not found
 *		- 'process_current_token()' processes if type is ARG or CMD
 *	   	- 'handle redirections()' processes if type is a redirection
 *			-- 'add_redirection()' processes the redirection
 *			-- 'handle_redirection_error()' handles errors if happen
 *		- 'process_other tokens()' processes rest of types: expandable vars,
 *			exit status, environment variables, heredoc
 */
void	handle_redirection_error(struct token *current, struct token **tokens)
{
	if ((current->prev && current->prev->type == T_PIPE) \
		|| (!current->next))
		ft_putstr_fd("minishell: syntax error near unexpected token `newline'", \
			2);
	else if ((current->next && current->next->type == T_PIPE) \
		|| (current->next && (current->next->type == T_REDIR_IN \
		|| current->next->type == T_APP_REDIR \
		|| current->next->type == T_REDIR_OUT)))
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
	else
		printf("Error: Expected filename after redirection symbol.\n");
	while (current && current->next)
		current = current->next;
	*tokens = current;
	g_exitstatus = 2;
}

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
		process_other_tokens(*current, cmd_node);
	*current = (*current)->next;
	return (0);
}

struct command_node	*parse_command(struct token **tokens)
{
	struct command_node	*cmd_node;
	struct token		*current;
	struct token		**last_arg;

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

// struct command_node	*parse_line(struct token *tokens)
// {
// 	struct command_node	*head;
// 	struct command_node	*prev;
// 	struct command_node	*current;

// 	head = NULL;
// 	prev = NULL;
// 	while (tokens)
// 	{
// 		current = parse_command(&tokens);
// 		if (!current)
// 			return (NULL);
// 		if (!head)
// 		{
// 			head = current;
// 			prev = head;
// 		}
// 		else
// 		{
// 			prev->next = current;
// 			prev = current;
// 		}
// 		if (tokens && tokens->type == T_PIPE)
// 		{
// 			if (!current || (!current->command && !current->redirects))
// 			{
// 				ft_putstr_fd("minishell: syntax error near unexpected token `|'"
// 					"\n", 2);
// 				g_exitstatus = 2;
// 				return (NULL);
// 			}
// 			tokens->prev = tokens;
// 			tokens = tokens->next;
// 		}
// 	}
// 	return (head);
// }
