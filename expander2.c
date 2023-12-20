/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkhrabro <vkhrabro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 20:55:03 by vkhrabro          #+#    #+#             */
/*   Updated: 2023/12/20 23:42:33 by vkhrabro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

struct token	*create_new_token(char *content, enum tokentype type)
{
	struct token	*new_token;

	new_token = (struct token *)malloc(sizeof(struct token));
	if (!new_token)
		return (NULL);
	new_token->content = ft_strdup(content);
	if (!new_token->content)
	{
		free(new_token);
		return (NULL);
	}
	new_token->type = type;
	new_token->next = NULL;
	return (new_token);
}

void	insert_new_token(struct command_node *current_command, char *env_value)
{
	enum tokentype		type;
	struct token		*new_token;

	if (current_command->command)
		type = T_ARG;
	else
		type = T_CMD;
	new_token = create_new_token(env_value, type);
	if (!new_token)
		return ;
	if (type == T_ARG)
	{
		new_token->next = current_command->args;
		current_command->args = new_token;
	}
	else
	{
		new_token->next = current_command->command;
		current_command->command = new_token;
	}
}

void	cleanup_old_data(struct command_node *current_command)
{
	if (current_command->var_expansion)
	{
		free(current_command->var_expansion->content);
		free(current_command->var_expansion);
		current_command->var_expansion = NULL;
	}
	if (current_command->env_variable)
	{
		free(current_command->env_variable->content);
		free(current_command->env_variable);
		current_command->env_variable = NULL;
	}
}

void	rpl_env_with_tkn(struct command_node *current_command,
		char *env_value)
{
	insert_new_token(current_command, env_value);
	cleanup_old_data(current_command);
}

void	handle_env_var(struct command_node *current_command,
			t_env_lst **env_lst)
{
	t_env_lst	*current_env;
	bool		found;

	current_env = *env_lst;
	found = false;
	while (current_env)
	{
		if (ft_strncmp(current_env->var_name,
				current_command->env_variable->content,
				ft_strlen(current_command->env_variable->content)) == 0)
		{
			if (current_env->var_value && *current_env->var_value)
			{
				rpl_env_with_tkn(current_command, current_env->var_value);
				found = true;
				break ;
			}
			else
			{
				g_exitstatus = 0;
				return ;
			}
		}
		current_env = current_env->next;
	}
}
