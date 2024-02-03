/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 17:44:14 by vadimhrabro       #+#    #+#             */
/*   Updated: 2024/01/31 15:18:57 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*append_line(char *existing_content, char *new_line)
{
	char	*result;

	if (!existing_content)
		return (ft_strdup(new_line));
	result = ft_strjoin(existing_content, new_line);
	return (result);
}

char	*read_heredoc_content(char *lim)
{
	char	*input_line;
	char	*all_content;
	char	*temp_content;
	char	*new_temp;

	all_content = NULL;
	set_heredoc_signals();
	while (1)
	{
		input_line = readline("> ");
		if (input_line == NULL)
			break ;
		if (ft_strncmp(input_line, lim, find_max_len(input_line, lim)) == 0)
		{
			free(input_line);
			break ;
		}
		temp_content = append_line(all_content, input_line);
		free(all_content);
		new_temp = ft_strjoin(temp_content, "\n");
		free(temp_content);
		all_content = new_temp;
		free(input_line);
	}
	return (all_content);
}

void	add_redirection(struct command_node *cmd_node, \
		struct token *redir_token, char *filename)
{
	struct redirection	*new_redir;
	struct redirection	*last;

	new_redir = malloc(sizeof(struct redirection));
	if (!new_redir)
	{
		perror("Failed to allocate memory for redirection");
		exit(EXIT_FAILURE);
	}
	new_redir->type = redir_token;
	new_redir->filename = strdup(filename);
	new_redir->next = NULL;
	if (cmd_node->redirects == NULL)
		cmd_node->redirects = new_redir;
	else
	{
		last = cmd_node->redirects;
		while (last->next != NULL)
			last = last->next;
		last->next = new_redir;
	}
}

void	process_other_tokens(struct token *current, \
	struct command_node *cmd_node, struct token ***last_arg)
{
	if (current->type == T_VAR_EXP)
	{
		cmd_node->var_expansion = current;
		*last_arg = &(current)->next;
	}
	else if (current->type == T_EX_ST)
	{
		cmd_node->ex_status = current;
		*last_arg = &(current)->next;
	}
	else if (current->type == T_ENV_VAR)
	{
		cmd_node->env_variable = current;
		*last_arg = &(current)->next;
	}
	else if (current->type == T_HEREDOC)
		process_heredoc_token(current, cmd_node);
}

int	handle_pipe_tokens(struct token **tokens, struct command_node **current)
{
	struct token	*pipe;

	if (*tokens && (*tokens)->type == T_PIPE)
	{
		pipe = *tokens;
		if (!(*current) || (!(*current)->command && !(*current)->redirects))
		{
			ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", \
				2);
			g_exitstatus = 2;
			return (0);
		}
		if (pipe->prev)
			pipe->prev->next = NULL;
		*tokens = pipe->next;
		free(pipe->content);
		free(pipe);
		pipe = NULL;
		return (1);
	}
	return (1);
}
