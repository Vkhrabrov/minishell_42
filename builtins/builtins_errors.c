/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_errors.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 19:43:51 by ccarrace          #+#    #+#             */
/*   Updated: 2023/11/09 20:19:03 by ccarrace         ###   ########.fr       */
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
	arg[len] = '"';
	arg[len + 1] = '\0';
	return (arg);
}

int	build_error_msg(char *command_name, char *arg, char *err_description, \
						bool quoted)
{
	char	*quoted_arg;
	char	*error_msg_first_part;
	char	*error_msg_second_part;
	char	*full_error_msg;

	error_msg_first_part = NULL;
	error_msg_second_part = NULL;
	full_error_msg = NULL;
	if (quoted == true)
	{
		quoted_arg = quote_arg(arg);
		error_msg_first_part = ft_strjoin("minishell: ", command_name);
		error_msg_second_part = ft_strjoin(error_msg_first_part, quoted_arg);
		full_error_msg = ft_strjoin(error_msg_second_part, err_description);
	}
	else
	{
		error_msg_first_part = ft_strjoin("minishell: ", command_name);
		error_msg_second_part = ft_strjoin(error_msg_first_part, arg);
		full_error_msg = ft_strjoin(error_msg_second_part, err_description);
	}
	write(STDERR_FILENO, full_error_msg, ft_strlen(full_error_msg));
	write(STDERR_FILENO, "\n", 1);
	free(error_msg_first_part);
	free(error_msg_second_part);
	free(full_error_msg);
	return(1);
}
