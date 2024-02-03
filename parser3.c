/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 22:25:08 by vkhrabro          #+#    #+#             */
/*   Updated: 2024/01/31 13:57:27 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	process_heredoc_token(struct token *current, \
		struct command_node *cmd_node)
{
	char	*delimiter;

	if (current->next && current->next->type == T_HEREDOC_DELIM)
	{
		cmd_node->redirect_in = current;
		delimiter = current->next->content;
		cmd_node->here_doc_content = read_heredoc_content(delimiter);
		current = current->next;
	}
}
