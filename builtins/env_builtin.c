/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 21:00:11 by ccarrace          #+#    #+#             */
/*   Updated: 2023/11/29 21:18:58 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
    'envp' is derived from "environment pointer".
    The 'envp' argument is automatically provided by the operating system.
*/
int	env_builtin(t_env_lst *env_lst, token *args_lst)
{
	t_env_lst	*current;

	if (ft_list_size(args_lst) > 0)
		return (build_error_msg("env: ", args_lst->content, \
				": No such file or directory", false));
	else
	{
		current = env_lst;
		while (current != NULL)
		{
			if (current->var_value != NULL)
				printf("%s=%s\n", current->var_name, current->var_value);
			current = current->next;
		}
	}
	return (0);
}
