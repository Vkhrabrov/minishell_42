/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkhrabro <vkhrabro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 21:35:50 by vkhrabro          #+#    #+#             */
/*   Updated: 2023/12/20 22:37:11 by vkhrabro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*extract_env_var_name(char *arg_content)
{
	char	*dollar;
	char	*start;
	int		i;
	char	*env_var_name;

	dollar = ft_strchr(arg_content, '$');
	if (dollar == NULL || ft_strchr(dollar, '|') == NULL)
		return (NULL);
	start = dollar;
	i = 0;
	while (*dollar != '|' && *dollar != '\0')
	{
		dollar++;
		i++;
	}
	env_var_name = malloc(sizeof(char) * (i + 1));
	if (!env_var_name)
		return (NULL);
	ft_strlcpy(env_var_name, start + 1, i);
	return (env_var_name);
}

void	replace_arg_with_env_value(command_node *current_command,
			t_env_lst **env_lst, char *env_var_name)
{
	t_env_lst	*current_env;
	char		*new_arg;

	if (!env_var_name)
		return ;
	current_env = *env_lst;
	new_arg = ft_strdup("|");
	while (current_env)
	{
		if (ft_strncmp(current_env->var_name, env_var_name,
				find_max_len(current_env->var_name, env_var_name)) == 0)
		{
			if (current_env->var_value && *current_env->var_value)
				new_arg = ft_strjoin(new_arg, current_env->var_value);
		}
		current_env = current_env->next;
	}
	new_arg = ft_strjoin(new_arg, "|");
	ft_strlcpy(current_command->args->content, new_arg, ft_strlen(new_arg) + 1);
	free(env_var_name);
	free(new_arg);
}

void	process_command_arguments(command_node *current_command,
			t_env_lst **env_lst)
{
	char	*env_var_name;

	env_var_name = extract_env_var_name(current_command->args->content);
	replace_arg_with_env_value(current_command, env_lst, env_var_name);
}

void	handle_exit_status(command_node *current_command)
{
	char	*exit_status_str;

	if (current_command->ex_status)
	{
		exit_status_str = ft_itoa(g_exitstatus);
		if (!exit_status_str)
			return ;
		replace_env_with_token(current_command, exit_status_str);
		free(exit_status_str);
	}
}

void	expand_environment_variables(command_node *cmds, t_env_lst **env_lst)
{
	command_node	*current_command;

	current_command = cmds;
	while (current_command)
	{
		if (current_command->env_variable)
			handle_environment_variable(current_command, env_lst);
		if (current_command->args)
			process_command_arguments(current_command, env_lst);
		handle_exit_status(current_command);
		current_command = current_command->next;
	}
}
