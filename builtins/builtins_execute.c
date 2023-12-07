/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_execute.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 20:36:56 by ccarrace          #+#    #+#             */
/*   Updated: 2023/12/05 00:11:44 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	execute_builtin(char *cmd_name, command_node *cmd_node, t_env_lst *env_lst)
{
	char	*name;
	int		exit_status;

	name = cmd_name;
	exit_status = 127;
	if (ft_strncmp(name, "echo", find_max_len(name, "echo")) == 0)
		exit_status = echo_builtin(env_lst, cmd_node->args);
	else if (ft_strncmp(name, "pwd", find_max_len(name, "pwd")) == 0)
		exit_status = pwd_builtin(env_lst);
	else if (ft_strncmp(name, "env", find_max_len(name, "env")) == 0)
		exit_status = env_builtin(env_lst, cmd_node->args);
	else if (ft_strncmp(name, "cd", find_max_len(name, "cd")) == 0)
		exit_status = cd_builtin(env_lst, cmd_node->args);
	else if (ft_strncmp(name, "export", find_max_len(name, "export")) == 0)
		exit_status = export_builtin(env_lst, cmd_node->args);
	else if (ft_strncmp(name, "unset", find_max_len(name, "unset")) == 0)
		exit_status = unset_builtin(&env_lst, cmd_node->args);
	else if (ft_strncmp(name, "exit", find_max_len(name, "exit")) == 0)
		exit_status = exit_builtin(cmd_node->args);
	g_exitstatus = exit_status;
	return (exit_status);
}
