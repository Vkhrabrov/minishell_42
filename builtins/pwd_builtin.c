/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 19:20:45 by ccarrace          #+#    #+#             */
/*   Updated: 2023/12/17 12:18:52 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	pwd_builtin(t_env_lst *env_lst)
{
	char	*path;

	path = get_env_var_val(env_lst, "PWD");
	printf("%s\n", path);
	return (0);
}
