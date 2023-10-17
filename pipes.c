/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkhrabro <vkhrabro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 23:49:10 by vkhrabro          #+#    #+#             */
/*   Updated: 2023/10/16 00:02:04 by vkhrabro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	child_process(t_pipex *tab, int *end, char **argv, char *envp[])
{
	close(end[0]);
	if (dup2(tab->in_fd, STDIN_FILENO) < 0)
		exit(clean_exit(tab, 1));
	if (dup2(end[1], STDOUT_FILENO) < 0)
		exit(clean_exit(tab, 1));
	close(tab->in_fd);
	close(end[1]);
	check_path(tab, argv[tab->b], envp);
	if (execve(tab->cmd, tab->cmd_args, envp) == -1)
		exit(clean_exit(tab, 1));
}

int	pipex_extension(t_pipex *tab, char **args, char **envp)
{
	if (dup2(tab->in_fd, STDIN_FILENO) < 0)
		exit(clean_exit(tab, -1));
	if (dup2(tab->out_fd, STDOUT_FILENO) < 0)
		exit(clean_exit(tab, 1));
	check_path(tab, args[tab->b], envp);
	if (execve(tab->cmd, tab->cmd_args, envp) == -1)
		exit(clean_exit(tab, 1));
	return (0);
}

int	pipex(t_pipex *tab, int argc, char **args, char **envp)
{
	int		end[2];
	pid_t	pid;

	if (tab->heredoc == 1)
		tab->b = 3;
	else
		tab->b = 2;
	while (tab->b < argc - 2)
	{
		if (pipe(end) < 0)
			exit(clean_exit(tab, 1));
		pid = fork();
		if (pid < 0)
			exit(clean_exit(tab, 1));
		else if (pid == 0)
			child_process(tab, end, args, envp);
		close(tab->in_fd);
		tab->in_fd = end[0];
		close(end[1]);
		tab->b++;
	}
	pipex_extension(tab, args, envp);
	return (0);
}