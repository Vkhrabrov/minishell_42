/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 13:40:46 by ccarrace          #+#    #+#             */
/*   Updated: 2023/12/10 22:58:21 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
// #include <ctype.h>

int	calculate_exit_status(long num)
{
	int	exit_status;

	if (num > 0)
		exit_status = num % 256;
	else if (num < 0)
		exit_status = 256 - ((num * -1) % 256);
	else
		exit_status = 0;
	return (exit_status);
}

int	handle_single_argument(token *args_lst)
{
	long	num;
	char	*str;
	int		exit_status;

	remove_leading_zeros(args_lst->content);
	remove_trailing_spaces(args_lst->content);
	if ((*args_lst->content == '+' && ft_isdigit(args_lst->content[1]))
		|| (*args_lst->content == ' ' && ft_isdigit(args_lst->content[1]))
		|| (*args_lst->content == '-' && args_lst->content[1] == '0'))
		args_lst->content++;
	if (*args_lst->content == '-' && args_lst->content[1] == '-' && !args_lst->content[2])
	{
		ft_putstr_fd("exit\n", 2);
		return (0);
	}
	num = ft_atol(args_lst->content);
	str = ft_ltoa(num);
	if ((num == 0 && is_valid_numeric(args_lst->content) == false)
		|| ft_strncmp(args_lst->content, str, 0xFFFFFF))
	{
		ft_putstr_fd("exit\n", 2); // only this line -> mpanic OK, bash KO
		build_error_msg("exit: ", args_lst->content, MS_NOTNUMARG, false);
		exit_status = 255;
	}
	else
	{
		exit_status = calculate_exit_status(num);
		ft_putstr_fd("exit\n", 2); // this line plus previous -> bash OK, mpanic KO
	}
	free(str);
	return (exit_status);
}

int	handle_multiple_arguments(token *args_lst)
{
	size_t	i;
	int		exit_status;

	i = 1;
	exit_status = 1;
	while (i < ft_strlen(args_lst->content))
	{
		if (!ft_isdigit(args_lst->content[i]))
		{
			ft_putstr_fd("exit\n", 2);
			build_error_msg("exit: ", args_lst->content, MS_NOTNUMARG, false);
			exit_status = 255;
			break;
		}
		i++;
	}
	return (exit_status);
}

int	exit_builtin(token *args_lst)
{
	int	exit_status;
	if (args_lst == NULL)
		exit(EXIT_SUCCESS);
	if (ft_list_size(args_lst) == 1)
		exit_status = handle_single_argument(args_lst);
	else
	{
		exit_status = handle_multiple_arguments(args_lst);
		if (exit_status != 255)
			return (build_error_msg("exit: ", NULL, MS_TOOMANYARG, false));
	}
	// free_args_list(args_lst);
	exit (exit_status);
}
