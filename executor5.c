/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor5.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkhrabro <vkhrabro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 19:32:19 by vkhrabro          #+#    #+#             */
/*   Updated: 2023/12/20 20:19:52 by vkhrabro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_here_doc(command_node *cmd_node)
{
	int	pipefd[2];

	if (!cmd_node->here_doc_content)
		return ;
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	write(pipefd[1], cmd_node->here_doc_content,
		ft_strlen(cmd_node->here_doc_content));
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
}

void	handle_infile(command_node *cmd_node)
{
	redirection	*redir;
	int			infd;

	redir = cmd_node->redirects;
	while (redir)
	{
		if (ft_strncmp(redir->type->content, "<", 2) == 0)
		{
			infd = open(redir->filename, O_RDONLY);
			if (infd == -1)
			{
				perror(redir->filename);
				exit(EXIT_FAILURE);
			}
			dup2(infd, STDIN_FILENO);
			close(infd);
			break ;
		}
		redir = redir->next;
	}
}

void	handle_outfile(command_node *cmd_node)
{
	redirection	*redir;
	int			outfd;
	int			flags;

	redir = cmd_node->redirects;
	while (redir)
	{
		if (ft_strncmp(redir->type->content, ">", 2) == 0
			|| ft_strncmp(redir->type->content, ">>", 3) == 0)
		{
			if (ft_strncmp(redir->type->content, ">>", 3) == 0)
				flags = O_APPEND | O_CREAT | O_RDWR;
			else
				flags = O_TRUNC | O_CREAT | O_RDWR;
			outfd = open(redir->filename, flags, 0644);
			if (outfd == -1)
			{
				perror(redir->filename);
				exit(EXIT_FAILURE);
			}
			dup2(outfd, STDOUT_FILENO);
			close(outfd);
		}
		redir = redir->next;
	}
}

void	execution_checks(command_node *cmd_node, t_exec_context *exec_ctx)
{
	if (!is_builtin(cmd_node))
	{
		if (!exec_ctx->full_cmd_path || *exec_ctx->full_cmd_path == '\0'
			|| cmd_node->command->content[0] == '\0')
		{
			build_error_msg(cmd_node->command->content,
				NULL, ": command not found", false);
			g_exitstatus = 127;
			exit(127);
		}
	}
	handle_infile(cmd_node);
	handle_outfile(cmd_node);
	handle_here_doc(cmd_node);
}

char	*find_command_path(const char *cmd, char **paths)
{
	char	*full_cmd_path;

	full_cmd_path = NULL;
	if (cmd[0] == '/' || cmd[0] == '.' || ft_strchr(cmd, '/'))
		full_cmd_path = check_command_accessibility(cmd);
	else
		full_cmd_path = search_command_in_paths(cmd, paths);
	return (full_cmd_path);
}
