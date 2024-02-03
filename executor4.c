/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 19:24:52 by vkhrabro          #+#    #+#             */
/*   Updated: 2024/01/31 23:04:15 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	child_process(struct command_node *cmd_node, t_env_lst *env_lst,
			int in_fd, int out_fd)
{
	int				original_stdout;
	int				original_stdin;
	struct token	*tokens;

	original_stdout = dup(STDOUT_FILENO);
	original_stdin = dup(STDIN_FILENO);
	tokens = NULL;
	if ((!cmd_node->command || !cmd_node->command->content)
		&& cmd_node->redirects)
		child_process_fd_handler(original_stdout, original_stdin, cmd_node);
	if (in_fd != STDIN_FILENO)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	if (out_fd != STDOUT_FILENO)
	{
		dup2(out_fd, STDOUT_FILENO);
		close(out_fd);
	}
	execute_command_node(cmd_node, env_lst, tokens);
	exit(g_exitstatus);
}

void	handle_child_process(struct command_node *current, t_env_lst *env_lst,
			int in_fd, int end[])
{
	int				out_fd;
	struct token	*tokens;

	tokens = NULL;
	if (current->next)
	{
		close(end[0]);
		out_fd = end[1];
	}
	else
		out_fd = STDOUT_FILENO;
	child_process(current, env_lst, in_fd, out_fd);
}

void	fd_pipex_change(int *in_fd, int *end, struct command_node **current)
{
	if (*in_fd != STDIN_FILENO)
		close(*in_fd);
	if ((*current)->next)
	{
		close(end[1]);
		*in_fd = end[0];
	}
	else
		*in_fd = *in_fd;
}

int	setup_and_pipe_loop(struct command_node *head, \
	struct command_node **current, int *in_fd, t_env_lst *env_lst)
{
	int		end[2];
	pid_t	pid;
	int		status;

	status = 0;
	*in_fd = STDIN_FILENO;
	*current = head;
	while (*current)
	{
		if ((*current)->next)
		{
			if (pipe(end) < 0)
				exit(EXIT_FAILURE);
		}
		pid = fork();
		if (pid < 0)
			exit(EXIT_FAILURE);
		else if (pid == 0)
			handle_child_process(*current, env_lst, *in_fd, end);
		fd_pipex_change(in_fd, end, current);
		*current = (*current)->next;
	}
	parent_process_actions(&status, pid);
	return (status);
}

int	pipex(struct command_node *head, t_env_lst *env_lst)
{
	struct command_node	*current;
	int					in_fd;
	int					status;

	status = setup_and_pipe_loop(head, &current, &in_fd, env_lst);
	return (final_cleanup_and_exit_status());
}
