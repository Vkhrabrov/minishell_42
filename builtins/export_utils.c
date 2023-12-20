/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 21:11:35 by ccarrace          #+#    #+#             */
/*   Updated: 2023/12/20 20:58:20 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_export_list(t_env_lst **env_lst)
{
	t_env_lst	*current;

	current = *env_lst;
	while (current != NULL)
	{
		printf("declare -x %s", current->var_name);
		if (current->var_value != NULL)
			printf("=\"%s\"\n", current->var_value);
		else
			printf("\n");
		current = current->next;
	}
}

/* is_var_name_valid()
 * 
 *	- Only alphabetical characters or underscore allowed 
 *		at the beginning of the name
 *	- Only alphanumerical characters or underscore allowed
 *		in the rest of the name
 */
bool	is_var_name_valid(char *arg, int equal_sign_position)
{
	int	i;

	i = 0;
	if (!ft_isalpha(arg[i]) && arg[i] != '_')
		return (false);
	i++;
	while (i < equal_sign_position)
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

t_env_lst	*insert_sorted(t_env_lst **head, t_env_lst *new_node)
{
	t_env_lst	*current;
	t_env_lst	*prev;

	current = *head;
	prev = NULL;
	while (current != NULL && ft_strncmp(new_node->var_name, current->var_name,
			find_max_len(new_node->var_name, current->var_name)) > 0)
	{
		prev = current;
		current = current->next;
	}
	if (prev == NULL)
	{
		new_node->next = *head;
		*head = new_node;
	}
	else
	{
		prev->next = new_node;
		new_node->next = current;
	}
	return (*head);
}

bool	find_var_name(t_env_lst *env_lst, char *arg)
{
	while (env_lst)
	{
		if (ft_strncmp(env_lst->var_name, arg, \
			find_max_len(env_lst->var_name, arg)) == 0)
			return (true);
		env_lst = env_lst->next;
	}
	return (false);
}

char	*return_var_value(t_env_lst *env_lst, char *arg)
{
	while (env_lst)
	{
		if (ft_strncmp(env_lst->var_name, arg, \
			find_max_len(env_lst->var_name, arg)) == 0)
			return (env_lst->var_value);
		env_lst = env_lst->next;
	}
	return (NULL);
}
