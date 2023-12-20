/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 21:11:32 by ccarrace          #+#    #+#             */
/*   Updated: 2023/12/20 19:54:06 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

const char	*token_type_to_string(enum tokentype type)
{
	if (type == T_CMD)
		return ("COMMAND");
	else if (type == T_ARG)
		return ("ARGUMENT");
	else if (type == T_PIPE)
		return ("PIPE");
	else if (type == T_REDIR_IN)
		return ("REDIRECT_IN");
	else if (type == T_REDIR_OUT)
		return ("REDIRECT_OUT");
	else if (type == T_HEREDOC)
		return ("HERE_DOC");
	else if (type == T_APP_REDIR)
		return ("APPEND_REDIRECTION");
	else if (type == T_VAR_EXP)
		return ("VARIABLE_EXPANSION");
	else if (type == T_EXIT_STATUS)
		return ("EXIT_STATUS");
	else if (type == T_HEREDOC_DELIM)
		return ("HEREDOC_DELIM");
	else if (type == T_ENV_VAR)
		return ("ENV_VARIABLE");
	else
		return ("UNKNOWN");
}

void	print_tokens(const struct token *tokens)
{
	while (tokens != NULL)
	{
		printf("Token: '%s', Type: %s\n", tokens->content, \
				token_type_to_string(tokens->type));
		tokens = tokens->next;
	}
}

void	print_redirections(struct command_node *node)
{
	struct redirection	*redir;

	redir = node->redirects;
	while (redir)
	{
		printf("  Redirection: %s\n  Filename: %s\n", redir->type->content, \
			redir->filename);
		redir = redir->next;
	}
	if (node->here_doc_content)
		printf("  HERE_DOC Content: %s\n", node->here_doc_content);
	printf("--------------------------\n");
}

void	print_command_node(struct command_node *node)
{
	struct token	*arg;

	while (node)
	{
		printf("Command Node:\n");
		if (node->command)
			printf("  Command: %s\n", node->command->content);
		arg = node->args;
		while (arg)
		{
			printf("  Argument: %s\n", arg->content);
			arg = arg->next;
		}
		if (node->var_expansion)
			printf("  Variable Expansion: %s\n", node->var_expansion->content);
		if (node->env_variable)
			printf("  Environment Variable: %s\n", node->env_variable->content);
		if (node->ex_status)
			printf("  Exit status: %s\n", node->ex_status->content);
		print_redirections(node);
		node = node->next;
	}
}

void	print_env_list(t_env_lst **env_lst)
{
	t_env_lst	*current;

	current = *env_lst;
	while (current != NULL)
	{
		printf("%s=%s\n", current->var_name, current->var_value);
		current = current->next;
	}
}
