/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 13:40:46 by ccarrace          #+#    #+#             */
/*   Updated: 2023/11/30 00:05:31 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
// #include <ctype.h>

void	calculate_exit_status(long num)
{
	if (num > 0)
		exit(num % 256);
	else if (num < 0)
		exit(256 - ((num * -1) % 256));
	else
		exit(EXIT_SUCCESS);
}

void	handle_single_argument(token *args_lst)
{
	long	num;
	char	*str;

	remove_leading_zeros(args_lst->content);
	remove_trailing_spaces(args_lst->content);
	if ((*args_lst->content == '+' && ft_isdigit(args_lst->content[1]))
		|| (*args_lst->content == ' ' && ft_isdigit(args_lst->content[1]))
		|| (*args_lst->content == '-' && args_lst->content[1] == '0'))
		args_lst->content++;
	num = ft_atol(args_lst->content);
	str = ft_ltoa(num);
	if ((num == 0 && is_valid_numeric(args_lst->content) == false)
		|| ft_strncmp(args_lst->content, str, 0xFFFFFF))
	{
		ft_putstr_fd("exit\n", 2);
		build_error_msg("exit: ", args_lst->content, MS_NOTNUMARG, false);
		exit(255);
	}
	else
		calculate_exit_status(num);
}

void	handle_multiple_arguments(token *args_lst)
{
	pid_t	pid;
	size_t	i;
	int		status;

	i = -1;
	while (++i < ft_strlen(args_lst->content))
	{
		if (!ft_isdigit(args_lst->content[i]))
		{
			ft_putstr_fd("exit\n", 2);
			build_error_msg("exit: ", args_lst->content, MS_NOTNUMARG, false);
			exit(255);
		}
	}
	pid = fork();
	if (pid == 0)
	{
		build_error_msg("exit: ", NULL, MS_TOOMANYARG, false);
		exit(1);
	}
	else
	{
		waitpid(pid, &status, 0);
		g_exitstatus = WEXITSTATUS(status);
	}
}

int	exit_builtin(token *args_lst)
{
	if (args_lst == NULL)
		exit(EXIT_SUCCESS);
	if (ft_list_size(args_lst) == 1)
		handle_single_argument(args_lst);
	else
		handle_multiple_arguments(args_lst);
	return (EXIT_FAILURE);
}
