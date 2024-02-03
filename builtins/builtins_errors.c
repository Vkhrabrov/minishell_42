/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_errors.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkhrabro <vkhrabro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 19:43:51 by ccarrace          #+#    #+#             */
/*   Updated: 2023/12/16 23:25:53 by vkhrabro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*quote_arg(char *arg)
{
	int	i;
	int	len;

	len = strlen(arg);
	i = len;
	while (i >= 0)
	{
		arg[i + 1] = arg[i];
		i--;
	}
	arg[0] = '`';
	len = strlen(arg);
	arg[len] = '\'';
	arg[len + 1] = '\0';
	return (arg);
}

void	print_and_free(char *error_msg_part_one, char *error_msg_part_two, \
						char *full_error_msg)
{
	ft_putstr_fd(full_error_msg, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	free(error_msg_part_one);
	free(error_msg_part_two);
	free(full_error_msg);
}

int	build_error_msg(char *command_name, char *arg, char *err_description, \
						bool quoted)
{
	char	*quoted_arg;
	char	*error_msg_part_one;
	char	*error_msg_part_two;
	char	*full_error_msg;

	error_msg_part_one = NULL;
	error_msg_part_two = NULL;
	full_error_msg = NULL;
	error_msg_part_one = ft_strjoin("minishell: ", command_name);
	if (arg != NULL)
	{
		if (quoted == true)
		{
			quoted_arg = quote_arg(arg);
			error_msg_part_two = ft_strjoin(error_msg_part_one, quoted_arg);
		}
		else
			error_msg_part_two = ft_strjoin(error_msg_part_one, arg);
		full_error_msg = ft_strjoin(error_msg_part_two, err_description);
	}
	else
		full_error_msg = ft_strjoin(error_msg_part_one, err_description);
	print_and_free(error_msg_part_one, error_msg_part_two, full_error_msg);
	return (EXIT_FAILURE);
}
