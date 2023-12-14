/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list_creation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 20:53:39 by vkhrabro          #+#    #+#             */
/*   Updated: 2023/12/15 00:51:20 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_var_with_value(const char *envp_line, t_env_lst *new_node, \
		size_t start_pos, size_t end_pos)
{
	new_node->var_name = ft_substr(envp_line, 0, start_pos - 1);
	if (ft_strncmp(new_node->var_name, "OLDPWD", \
			find_max_len(new_node->var_name, "OLDPWD")) == 0)
		new_node->var_value = NULL;
	else
		new_node->var_value = ft_substr(envp_line, start_pos, end_pos);
}

/*	create_env_node()
 *
 *	for each line in the array of environment variables (envp)
 *	- it creates a node 
 *	- finds the position of the equal character (=)
 *		-- if there is no equal, stores the name of the variable
 *			in the node, and sets NULL as its value
 *		-- if there is an equal, stores name and value in the
 *			node (see 'add_var_with_value()' function above)
 */
t_env_lst	*create_env_node(const char *envp_line)
{
	t_env_lst	*new_node;
	size_t		equal_sign_position;
	size_t		envp_line_len;

	new_node = malloc(sizeof(t_env_lst));
	if (new_node == NULL)
		return (NULL);
	equal_sign_position = find_char_index(envp_line, '=');
	envp_line_len = ft_strlen(envp_line);
	if (equal_sign_position == envp_line_len)
	{
		new_node->var_name = ft_strdup(envp_line);
		new_node->var_value = NULL;
	}
	else
		add_var_with_value(envp_line, new_node, equal_sign_position + 1, \
			envp_line_len - equal_sign_position);
	new_node->next = NULL;
	return (new_node);
}

void	append_to_list(t_env_lst **head, t_env_lst *new_node)
{
	t_env_lst	*current;

	if (*head == NULL)
		*head = new_node;
	else
	{
		current = *head;
		while (current->next != NULL)
			current = current->next;
		current->next = new_node;
	}
}

void	add_env_var_to_list(t_env_lst **head, const char *envp_line)
{
	t_env_lst	*new_node;

	new_node = create_env_node(envp_line);
	if (new_node == NULL)
		return ;
	append_to_list(head, new_node);
}

void	save_env_list(t_env_lst **env_lst, char **envp)
{
	int	i;

	i = 0;
	while (envp[i] != NULL)
	{
		add_env_var_to_list(env_lst, envp[i]);
		i++;
	}
}
