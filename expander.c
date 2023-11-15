/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkhrabro <vkhrabro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 21:35:50 by vkhrabro          #+#    #+#             */
/*   Updated: 2023/11/12 20:18:37 by vkhrabro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

token	*create_new_token(char *content, tokentype type)
{
	token	*new_token;

	new_token = (token *)malloc(sizeof(token));
	new_token->content = ft_strdup(content);
	new_token->type = type;
	new_token->next = NULL;
	return (new_token);
}

void	replace_env_with_token(command_node *current_command, char *env_value)
{
	tokentype	type;
	token		*new_token;

	type = T_CMD;
	if (current_command->command)
		type = T_ARG;
	new_token = create_new_token(env_value, type);
	if (new_token->type == T_ARG)
	{
		new_token->next = current_command->args;
		current_command->args = new_token;
	}
	else
		current_command->command = new_token;
	free(current_command->var_expansion->content);
	free(current_command->var_expansion);
	current_command->var_expansion = NULL;
	free(current_command->env_variable->content);
	free(current_command->env_variable);
	current_command->env_variable = NULL;
}

void	expand_environment_variables(command_node *cmds, t_env_lst **env_lst)
{
	command_node	*current_command;
	t_env_lst		*current_env;

	current_command = cmds;
	while (current_command)
	{
		if (current_command->env_variable)
		{
			current_env = *env_lst;
			while (current_env)
			{
				if (ft_strncmp(current_env->var_name,
						current_command->env_variable->content,
						ft_strlen(current_command->env_variable->content))
					== 0)
				{
					replace_env_with_token(current_command,
						current_env->var_value);
					break ;
				}
				current_env = current_env->next;
			}
		}
		// else if (current_command->env_variable)
		current_command = current_command->next;
	}
}
