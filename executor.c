/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 23:30:07 by vkhrabro          #+#    #+#             */
/*   Updated: 2024/01/31 23:20:31 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_paths(char **paths)
{
	int	i;

	i = 0;
	if (paths != NULL)
	{
		while (paths[i] != NULL)
		{
			free(paths[i]);
			i++;
		}
		free(paths);
	}
}

void	free_execution_resources(t_exec_context *exec_ctx)
{
	int	i;

	i = 0;
	if (exec_ctx->full_cmd_path != NULL)
		free(exec_ctx->full_cmd_path);
	if (exec_ctx->paths != NULL)
		free_paths(exec_ctx->paths);
	if (exec_ctx->final_args != NULL)
	{
		while (exec_ctx->final_args[i] != NULL)
			free(exec_ctx->final_args[i++]);
		free(exec_ctx->final_args);
	}
	i = 0;
	if (exec_ctx->final_env != NULL)
	{
		while (exec_ctx->final_env[i] != NULL)
			free(exec_ctx->final_env[i++]);
		free(exec_ctx->final_env);
	}
}

int	execute_command_node(struct command_node *cmd_node, t_env_lst *env_lst, \
	struct token *tokens)
{
	t_exec_context	exec_ctx;

	(void)tokens;
	exec_ctx.paths = get_paths_from_env(env_lst);
	exec_ctx.full_cmd_path = find_command_path(cmd_node->command->content,
			exec_ctx.paths);
	exec_ctx.final_args = cnv_cmd_nd_arg_to_arr(cmd_node);
	exec_ctx.final_env = convert_env_list_to_array(env_lst);
	exec_ctx.pid = fork();
	if (exec_ctx.pid == 0)
	{
		execution_checks(cmd_node, &exec_ctx);
		if (is_builtin(cmd_node))
			execute_builtin(cmd_node->command->content, cmd_node, env_lst);
		else
			execve(exec_ctx.full_cmd_path, exec_ctx.final_args,
				exec_ctx.final_env);
	}
	else
		parent_process_handler(&exec_ctx);
	free_execution_resources(&exec_ctx);
	return (g_exitstatus);
}

int	setup_and_count_nodes(struct command_node *head,
		int *original_stdout, int *original_stdin)
{
	struct command_node	*current;
	int					node_count;

	*original_stdout = dup(STDOUT_FILENO);
	*original_stdin = dup(STDIN_FILENO);
	node_count = 0;
	current = head;
	while (current)
	{
		node_count++;
		current = current->next;
	}
	return (node_count);
}

int	process_command_list(struct command_node *head, t_env_lst *env_lst, \
	struct token *tokens)
{
	int	original_stdout;
	int	original_stdin;
	int	node_count;
	int	return_status;

	if (!head)
		return (0);
	return_status = 0;
	node_count = setup_and_count_nodes(head, &original_stdout, &original_stdin);
	if ((!head->command || !head->command->content)
		&& !head->redirects && !head->here_doc_content)
		return_status = g_exitstatus;
	else if ((!head->command || !head->command->content)
		&& (head->redirects && node_count < 2))
		child_process_fd_handler(original_stdout, original_stdin, head);
	else if (node_count > 1)
		return_status = pipex(head, env_lst);
	else
		return_status = execute_or_builtin(head, env_lst, tokens);
	free_command(head, tokens);
	return (return_status);
}
