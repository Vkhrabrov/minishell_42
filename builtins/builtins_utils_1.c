/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils_1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 11:36:02 by ccarrace          #+#    #+#             */
/*   Updated: 2023/12/20 14:07:11 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* find_char_index() called in
	echo_builtin()		env_list_creation()		export_builtin()	
*/
size_t	find_char_index(const char *str, char target)
{
	size_t	index;

	index = 0;
	while (str[index] != '\0')
	{
		if (str[index] == target)
			return (index);
		index++;
	}
	return (0);
}

/* find_max_len() called in
	env_list_creation.c	executor.c		parser.c		builtins_execute.c
	cd_builtin.c		echo_builtin.c	env_builtin.c	exit_builtin.c
*/
int	find_max_len(char *str1, char *str2)
{
	int	len1;
	int	len2;
	int	max_len;

	len1 = ft_strlen(str1);
	len2 = ft_strlen(str2);
	if (len1 > len2)
		max_len = len1;
	else
		max_len = len2;
	return (max_len);
}

/* ft_list_size() called in
	env_builtin()	exit_builtini()		export_builtin()
*/
int	ft_list_size(struct token *args_lst)
{
	int	i;

	i = 0;
	while (args_lst != NULL)
	{
		i++;
		args_lst = args_lst->next;
	}
	return (i);
}

/* get_env_var_val() called in
	cd_builtin.c	echo_builtin.c		pwd_builtin.c
*/
char	*get_env_var_val(t_env_lst *env_lst, char *str)
{
	while (env_lst != NULL)
	{
		if (ft_strncmp(env_lst->var_name, str, \
			find_max_len(env_lst->var_name, str)) == 0)
			return (env_lst->var_value);
		env_lst = env_lst->next;
	}
	return (str);
}

/* update_env_var_value() called in
	cd_builtin()
*/
int	update_env_var_value(t_env_lst *env_lst, char *sought_name, \
	char *new_value)
{
	t_env_lst	*temp_ptr;

	temp_ptr = env_lst;
	while (temp_ptr)
	{
		if (ft_strncmp(temp_ptr->var_name, sought_name, \
			find_max_len(temp_ptr->var_name, sought_name)) == 0)
		{
			free(temp_ptr->var_value);
			temp_ptr->var_value = ft_strdup(new_value);
			return (0);
		}
		temp_ptr = temp_ptr->next;
	}
	return (1);
}
