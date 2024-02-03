/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor6.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 19:37:22 by vkhrabro          #+#    #+#             */
/*   Updated: 2024/01/31 23:23:22 by ccarrace         ###   ########.fr       */
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

void	parent_process_handler(t_exec_context *exec_ctx)
{
	waitpid(exec_ctx->pid, &exec_ctx->status, 0);
	if (WIFEXITED(exec_ctx->status))
		g_exitstatus = WEXITSTATUS(exec_ctx->status);
}

int	execute_or_builtin(struct command_node *head, t_env_lst *env_lst, \
	struct token *tokens)
{
	int	return_status;

	if (is_builtin(head))
		return_status = builtin_process(head, env_lst, tokens);
	else
		return_status = execute_command_node(head, env_lst, tokens);
	return (return_status);
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
