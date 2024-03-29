/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 13:40:46 by ccarrace          #+#    #+#             */
/*   Updated: 2024/01/31 01:31:04 by ccarrace         ###   ########.fr       */
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

static int	normalize_argument_and_check_double_dash(struct token *args_lst)
{
	remove_leading_zeros(args_lst->content);
	remove_trailing_spaces(args_lst->content);
	if ((*args_lst->content == '+' && ft_isdigit(args_lst->content[1]))
		|| (*args_lst->content == ' ' && ft_isdigit(args_lst->content[1]))
		|| (*args_lst->content == '-' && args_lst->content[1] == '0'))
		args_lst->content++;
	if (*args_lst->content == '-' && args_lst->content[1] == '-' \
		&& !args_lst->content[2])
	{
		ft_putendl_fd("exit", 0);
		return (1);
	}
	return (0);
}

/*	handle_single_argument()
 *
 *	I have to print 'exit' message through STDINPUT to please mpanic test.
 *	It does not allow me to print it through either STDOUTPUT or STDERR.
 */
int	handle_single_argument(struct token *args_lst)
{
	long	num;
	char	*str;
	int		exit_status;

	if (normalize_argument_and_check_double_dash(args_lst))
		return (0);
	num = ft_atol(args_lst->content);
	str = ft_ltoa(num);
	if ((num == 0 && is_valid_numeric(args_lst->content) == false)
		|| ft_strncmp(args_lst->content, str, 0xFFFFFF))
	{
		ft_putendl_fd("exit", 0);
		build_error_msg("exit: ", args_lst->content, MS_NOTNUMARG, false);
		exit_status = 255;
	}
	else
	{
		exit_status = calculate_exit_status(num);
		ft_putendl_fd("exit", 0);
	}
	free(str);
	return (exit_status);
}

int	handle_multiple_arguments(struct token *args_lst)
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
			break ;
		}
		i++;
	}
	return (exit_status);
}

int	exit_builtin(struct token *args_lst, struct command_node *head)
{
	int	exit_status;

	(void)head;
	if (args_lst == NULL)
	{
		ft_putstr_fd("exit\n", 0);
		exit(EXIT_SUCCESS);
	}
	else if (ft_list_size(args_lst) == 1)
		exit_status = handle_single_argument(args_lst);
	else
	{
		exit_status = handle_multiple_arguments(args_lst);
		if (exit_status != 255)
			return (build_error_msg("exit: ", NULL, MS_TOOMANYARG, false));
	}
	exit (exit_status);
}
