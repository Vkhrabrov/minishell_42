/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 21:01:39 by ccarrace          #+#    #+#             */
/*   Updated: 2024/01/31 01:17:43 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_env_list(t_env_lst *env_lst)
{
	t_env_lst	*temp;

	while (env_lst != NULL)
	{
		temp = env_lst;
		env_lst = env_lst->next;
		free(temp->var_name);
		if (temp->var_value != NULL)
			free(temp->var_value);
		free(temp);
	}
}

void	free_token(struct token *token)
{
	free(token->content);
	free(token);
}

void	free_token_list(struct token **token)
{
	struct token	*next;

	while (*token != NULL)
	{
		next = (*token)->next;
		if ((*token)->content)
		{
			free((*token)->content);
			(*token)->content = NULL;
		}
		free(*token);
		*token = NULL;
		if (next != NULL)
		{
			*token = next;
			next = NULL;
		}
		else
			break ;
	}
	*token = NULL;
}

void	free_redirection_list(struct redirection **redir)
{
	struct redirection	*next;

	while (*redir != NULL)
	{
		next = (*redir)->next;
		if ((*redir)->filename)
		{
			free((*redir)->filename);
			(*redir)->filename = NULL;
		}
		free(*redir);
		*redir = NULL;
		if (next != NULL)
		{
			*redir = next;
			next = NULL;
		}
		else
			break ;
	}
	*redir = NULL;
}

void	free_command(t_command_node *cmd_node, struct token *tokens)
{
	t_command_node	*cmd_node_current;
	t_command_node	*cmd_node_next;

	(void)tokens;
	cmd_node_current = cmd_node;
	while (cmd_node_current && cmd_node_current->command
		&& cmd_node_current->command->content != NULL)
	{
		cmd_node_next = cmd_node_current->next;
		free(cmd_node_current->redirect_in_filename);
		free(cmd_node_current->redirect_out_filename);
		free(cmd_node_current->here_doc_content);
		free_token_list(&cmd_node_current->command);
		free_redirection_list(&cmd_node_current->redirects);
		free(cmd_node_current);
		cmd_node_current = cmd_node_next;
	}
}
