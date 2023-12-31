/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 17:44:14 by vadimhrabro       #+#    #+#             */
/*   Updated: 2023/12/20 21:06:06 by ccarrace         ###   ########.fr       */
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

char	*read_heredoc_content(const char *delimiter)
{
	char	*input_line;
	char	*all_content;
	char	*temp_content;

	all_content = NULL;
	while (1)
	{
		input_line = readline("> ");
		if (ft_strncmp(input_line, (char *)delimiter, find_max_len(input_line, \
			(char *)delimiter)) == 0)
		{
			free(input_line);
			break ;
		}
		temp_content = append_line(all_content, input_line);
		temp_content = ft_strjoin(temp_content, "\n");
		free(all_content);
		all_content = temp_content;
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
		struct command_node *cmd_node)
{
	char	*delimiter;

	if (current->type == T_VAR_EXP)
		cmd_node->var_expansion = current;
	else if (current->type == T_EXIT_STATUS)
		cmd_node->ex_status = current;
	else if (current->type == T_ENV_VAR)
		cmd_node->env_variable = current;
	else if (current->type == T_HEREDOC)
	{
		if (current->next && current->next->type == T_HEREDOC_DELIM)
		{
			cmd_node->redirect_in = current;
			delimiter = current->next->content;
			cmd_node->here_doc_content = read_heredoc_content(delimiter);
			current = current->next;
		}
	}
}

int	handle_pipe_tokens(struct token **tokens, struct command_node **current)
{
	if (*tokens && (*tokens)->type == T_PIPE)
	{
		if (!(*current) || (!(*current)->command && !(*current)->redirects))
		{
			ft_putstr_fd("minishell: syntax error near unexpected token `|'"
				"\n", 2);
			g_exitstatus = 2;
			return (0);
		}
		(*tokens)->prev = *tokens;
		*tokens = (*tokens)->next;
		return (1);
	}
	return (1);
}
