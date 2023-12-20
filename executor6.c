/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor6.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkhrabro <vkhrabro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 19:37:22 by vkhrabro          #+#    #+#             */
/*   Updated: 2023/12/20 23:29:41 by vkhrabro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	child_process_fd_handler(int original_stdout, int original_stdin,
		struct command_node *cmd_node)
{
	handle_outfile(cmd_node);
	handle_infile(cmd_node);
	dup2(original_stdout, STDOUT_FILENO);
	dup2(original_stdin, STDIN_FILENO);
	close(original_stdout);
	close(original_stdin);
	exit(g_exitstatus);
}

void	parent_process_actions(int *status, pid_t pid)
{
	waitpid(pid, status, 0);
	if (WIFEXITED(*status))
		g_exitstatus = WEXITSTATUS(*status);
}

int	final_cleanup_and_exit_status(void)
{
	pid_t	pid;

	while (1)
	{
		pid = waitpid(-1, NULL, 0);
		if (pid <= 0)
		{
			if (errno == ECHILD)
				break ;
		}
	}
	return (g_exitstatus);
}
