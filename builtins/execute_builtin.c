/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 20:36:56 by ccarrace          #+#    #+#             */
/*   Updated: 2023/10/29 22:45:32 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	execute_builtin(char *cmd_name, command_node *cmd_node, t_env_lst env_lst)
{
	int	ret;
	int	cmd_len;

	cmd_len = ft_strlen(cmd_name);
	ret = 127;	//	bash's exit status value if command not found
	if (ft_strncmp(cmd_name, "echo", cmd_len) == 0)	
		ret = echo_builtin(cmd_node->args);
	else if (ft_strncmp(cmd_name, "pwd", cmd_len) == 0)
		ret = pwd_builtin(cmd_node->args);
	else if (ft_strncmp(cmd_name, "env", cmd_len) == 0)
		ret = env_builtin(&env_lst);
	else if (ft_strncmp(cmd_name, "cd", cmd_len) == 0)
		ret = cd_builtin(cmd_node->args);
	else if (ft_strncmp(cmd_name, "export", cmd_len) == 0)
		ret = export_builtin(&env_lst);
	return (ret);
}
