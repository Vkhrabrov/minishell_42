/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkhrabro <vkhrabro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 20:31:20 by vkhrabro          #+#    #+#             */
/*   Updated: 2023/12/20 19:59:06 by vkhrabro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**convert_command_node_args_to_array(command_node *cmd_node)
{
	int		count;
	char	**args_array;
	token	*arg_temp;
	int		i;

	count = 0;
	arg_temp = cmd_node->args;
	i = 1;
	while (arg_temp)
	{
		count++;
		arg_temp = arg_temp->next;
	}
	args_array = malloc(sizeof(char *) * (count + 2));
	args_array[0] = ft_strdup(cmd_node->command->content);
	arg_temp = cmd_node->args;
	while (arg_temp)
	{
		args_array[i] = ft_strdup(arg_temp->content);
		i++;
		arg_temp = arg_temp->next;
	}
	args_array[i] = NULL;
	return (args_array);
}

int	count_env_list_elements(t_env_lst *env_lst)
{
	int			count;
	t_env_lst	*temp;

	count = 0;
	temp = env_lst;
	while (temp)
	{
		count++;
		temp = temp->next;
	}
	return (count);
}

char	**convert_env_list_to_array(t_env_lst *env_lst)
{
	int		count;
	char	**env_array;
	int		i;
	char	*name_value;
	char	*tmp;

	count = count_env_list_elements(env_lst);
	env_array = malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);
	i = 0;
	while (env_lst)
	{
		name_value = ft_strjoin(env_lst->var_name, "=");
		if (env_lst->var_value)
		{
			tmp = name_value;
			name_value = ft_strjoin(name_value, env_lst->var_value);
			free(tmp);
		}
		env_array[i++] = name_value;
		env_lst = env_lst->next;
	}
	env_array[i] = NULL;
	return (env_array);
}

char	*get_env_value(t_env_lst *env_lst, const char *key)
{
	while (env_lst)
	{
		if (ft_strncmp(env_lst->var_name, (char *)key,
				ft_strlen(env_lst->var_name)) == 0)
			return (env_lst->var_value);
		env_lst = env_lst->next;
	}
	return (NULL);
}

char	**get_paths_from_env(t_env_lst *env_lst)
{
	char	*path_string;

	path_string = get_env_value(env_lst, "PATH");
	if (!path_string)
		return (NULL);
	return (ft_split(path_string, ':'));
}
