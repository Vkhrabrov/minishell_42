/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_execute.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkhrabro <vkhrabro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 20:36:56 by ccarrace          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2023/11/21 20:58:56 by vkhrabro         ###   ########.fr       */
=======
/*   Updated: 2023/12/03 12:40:46 by ccarrace         ###   ########.fr       */
>>>>>>> origin/master
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	execute_builtin(char *cmd_name, command_node *cmd_node, t_env_lst *env_lst)
{
<<<<<<< HEAD
	// pid_t pid = fork();

	// if (pid == 0)
	// {
		int	exit_status;

		exit_status = 127;	//	bash's exit status value if command not found
		if (ft_strncmp(cmd_name, "echo", find_max_len(cmd_name, "echo")) == 0)	
			exit_status = echo_builtin(env_lst, cmd_node->args);
		else if (ft_strncmp(cmd_name, "pwd", find_max_len(cmd_name, "pwd")) == 0)
			exit_status = pwd_builtin(env_lst);
		else if (ft_strncmp(cmd_name, "env", find_max_len(cmd_name, "env")) == 0)
			exit_status = env_builtin(env_lst, cmd_node->args);
		else if (ft_strncmp(cmd_name, "cd", find_max_len(cmd_name, "cd")) == 0)
			exit_status = cd_builtin(env_lst, cmd_node->args);
		else if (ft_strncmp(cmd_name, "export", find_max_len(cmd_name, "export")) == 0)
			exit_status = export_builtin(env_lst, cmd_node->args);	
		else if (ft_strncmp(cmd_name, "unset", find_max_len(cmd_name, "unset")) == 0)
			exit_status = unset_builtin(&env_lst, cmd_node->args);
		else if (ft_strncmp(cmd_name, "exit", find_max_len(cmd_name, "exit")) == 0)
			exit_status = exit_builtin(cmd_node->args);
	// printf("exit_status received = [%d]\n", exit_status);
		// return (exit_status);
		g_exitstatus = exit_status;
		return (exit_status);
	// }
	// else
	// {
	// 	int status;
    //     waitpid(pid, &status, 0);
	// 	g_exitstatus = WEXITSTATUS(status);
	// 	printf("parent status = %d\n", g_exitstatus);
	// }
=======
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
>>>>>>> origin/master
}
