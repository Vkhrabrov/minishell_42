/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 20:36:56 by ccarrace          #+#    #+#             */
/*   Updated: 2023/10/29 11:11:11 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	execute_builtin(char *cmd_name, command_node *cmd_node)
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
		ret = env_builtin(cmd_node->args);
	else if (ft_strncmp(cmd_name, "cd", cmd_len) == 0)
	{
		printf("sending '%s' string to cd_builtin\n", cmd_name);
		// ret = cd_builtin();
	}
	return (ret);
}
