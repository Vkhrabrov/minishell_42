/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkhrabro <vkhrabro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 19:20:12 by vkhrabro          #+#    #+#             */
/*   Updated: 2023/12/20 20:05:33 by vkhrabro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*concatenate_paths_libft(const char *path, const char *cmd)
{
	char	*slash;
	char	*temp1;
	char	*result;

	slash = "/";
	temp1 = ft_strjoin(path, slash);
	if (!temp1)
	{
		perror("ft_strjoin");
		exit(EXIT_FAILURE);
	}
	result = ft_strjoin(temp1, cmd);
	if (!result)
	{
		perror("ft_strjoin");
		exit(EXIT_FAILURE);
	}
	free(temp1);
	return (result);
}

int	is_builtin(command_node *cmd_node)
{
	char	*builtins[8];
	int		i;

	builtins[0] = "cd";
	builtins[1] = "echo";
	builtins[2] = "env";
	builtins[3] = "exit";
	builtins[4] = "export";
	builtins[5] = "pwd";
	builtins[6] = "unset";
	builtins[7] = NULL;
	i = 0;
	while (builtins[i] != NULL)
	{
		if (ft_strncmp(cmd_node->command->content, builtins[i],
				find_max_len(cmd_node->command->content, builtins[i])) == 0)
			return (1);
		i++;
	}
	return (0);
}

char	*check_command_accessibility(const char *cmd)
{
	char	*full_cmd_path;

	if (access(cmd, X_OK) == 0)
	{
		full_cmd_path = ft_strdup(cmd);
		if (!full_cmd_path)
		{
			perror("ft_strdup");
			exit(EXIT_FAILURE);
		}
		return (full_cmd_path);
	}
	return (NULL);
}

int	builtin_process(command_node *cmd_node, t_env_lst *env_lst)
{
	int	original_stdout_fd;
	int	ex_status;

	original_stdout_fd = -1;
	ex_status = 0;
	if (cmd_node->redirects)
	{
		original_stdout_fd = dup(STDOUT_FILENO);
		handle_outfile(cmd_node);
	}
	ex_status = execute_builtin(cmd_node->command->content, cmd_node, env_lst);
	if (original_stdout_fd != -1)
	{
		dup2(original_stdout_fd, STDOUT_FILENO);
		close(original_stdout_fd);
	}
	return (ex_status);
}

char	*search_command_in_paths(const char *cmd, char **paths)
{
	int		i;
	char	*temp_path;

	i = 0;
	while (paths && paths[i])
	{
		temp_path = concatenate_paths_libft(paths[i], cmd);
		if (!temp_path)
		{
			perror("malloc");
			exit(EXIT_FAILURE);
		}
		if (access(temp_path, X_OK) == 0)
			return (temp_path);
		free(temp_path);
		i++;
	}
	return (NULL);
}
