/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/16 20:07:30 by ccarrace          #+#    #+#             */
/*   Updated: 2023/12/17 21:11:59 by ccarrace         ###   ########.fr       */
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


void	handle_redirection_error(token *current, token **tokens)
{
	if ((current->prev && current->prev->type == T_PIPE) \
		|| (!current->next))
		ft_putstr_fd("minishell: syntax error near unexpected token `newline'", 2);
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

token	*handle_redirections(command_node *cmd_node, token *current, \
							token **last_arg, token **tokens)
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

int	process_current_token(token **current, command_node *cmd_node, \
							token ***last_arg, token **tokens)
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
	return(0);
}

command_node	*parse_command(token **tokens)
{
	command_node	*cmd_node;
	token			*current;
	token			**last_arg;

	if (!tokens || !(*tokens))
		return (NULL);
	cmd_node = ft_calloc(1, sizeof(command_node));
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

command_node	*parse_line(token *tokens)
{
    command_node *head;
    command_node *prev;

	head = NULL;
	prev = NULL;
    while (tokens) 
    {
        command_node *current = parse_command(&tokens);
        if (!current)
            return(NULL);
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
        // Check for invalid command before a pipe
        if (tokens && tokens->type == T_PIPE) 
        {
            if (!current || (!current->command && !current->redirects)) {
                ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
                g_exitstatus = 2;
                break; // Example: stop parsing further
            }
            tokens->prev = tokens;
            tokens = tokens->next; // Move past the pipe token
        }
    }
    return (head);
}
