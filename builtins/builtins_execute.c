/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_execute.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 20:36:56 by ccarrace          #+#    #+#             */
/*   Updated: 2023/11/14 21:14:13 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	find_max_len(char *str1, char *str2)
{
	int	len1;
	int	len2;
	int	max_len;

	len1 = ft_strlen(str1);
	len2 = ft_strlen(str2);
	if (len1 > len2)
		max_len = len1;
	else
		max_len = len2;
	return (max_len);
}

int	execute_builtin(char *cmd_name, command_node *cmd_node, t_env_lst *env_lst)
{
	int	exit_status;

	exit_status = 127;	//	bash's exit status value if command not found
	if (ft_strncmp(cmd_name, "echo", find_max_len(cmd_name, "echo")) == 0)	
		exit_status = echo_builtin(cmd_node->args);
	else if (ft_strncmp(cmd_name, "pwd", find_max_len(cmd_name, "pwd")) == 0)
		exit_status = pwd_builtin(cmd_node->args);
	else if (ft_strncmp(cmd_name, "env", find_max_len(cmd_name, "env")) == 0)
		exit_status = env_builtin(env_lst);
	else if (ft_strncmp(cmd_name, "cd", find_max_len(cmd_name, "cd")) == 0)
		exit_status = cd_builtin(env_lst, cmd_node->args);
	else if (ft_strncmp(cmd_name, "export", find_max_len(cmd_name, "export")) == 0)
		exit_status = export_builtin(env_lst, cmd_node->args);	
	else if (ft_strncmp(cmd_name, "unset", find_max_len(cmd_name, "unset")) == 0)
		exit_status = unset_builtin(&env_lst, cmd_node->args);
	else if (ft_strncmp(cmd_name, "exit", find_max_len(cmd_name, "exit")) == 0)
		exit_status = exit_builtin(cmd_node->args);
// printf("exit_status received = [%d]\n", exit_status);
	return (exit_status);
}
