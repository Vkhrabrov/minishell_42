/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 21:00:11 by ccarrace          #+#    #+#             */
/*   Updated: 2023/12/03 13:57:28 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	update_existing_var(t_env_lst **head, char *arg, \
			size_t equal_sign_position, size_t arg_len)
{
	if (equal_sign_position == arg_len)
	{
		if ((*head)->var_value != NULL)
		{
			free((*head)->var_value);
			(*head)->var_value = NULL;
		}
	}
	else
	{
		free((*head)->var_value);
		(*head)->var_value = ft_strdup(arg + equal_sign_position + 1);
	}
}

static void	add_new_var(t_env_lst **head, char *arg, size_t equal_sign_position)
{
	t_env_lst	*new_node;

	new_node = malloc(sizeof(t_env_lst));
	if (new_node == NULL)
		return ;
	if (equal_sign_position == 0)
	{
		new_node->var_name = ft_strdup(arg);
		new_node->var_value = NULL;
	}
	else
	{
		new_node->var_name = ft_substr(arg, 0, equal_sign_position);
		new_node->var_value = ft_strdup(arg + equal_sign_position + 1);
	}
	new_node->next = *head;
	*head = new_node;
}

/* set_new_var()
 *
 *	- Searches for an existing node with the same name as provided in arg
 *	- If a node with the same name exists, updates its value
 *		-- If no value is provided in arg, sets it to NULL
 *	- If the node is already updated, exits the function
 *	- If no node exists with the same name as provided in arg, creates it
 */
static int	set_new_var(t_env_lst **head, char *arg)
{
	size_t	equal_sign_position;
	size_t	arg_len;

	equal_sign_position = find_char_index(arg, '=');
	arg_len = ft_strlen(arg);
	if (!is_var_name_valid(arg, equal_sign_position))
		return (build_error_msg("export: ", arg, MS_BADID, true));
	while (*head != NULL)
	{
		if (ft_strncmp((*head)->var_name, arg, equal_sign_position) == 0
			&& (*head)->var_name[equal_sign_position] == '\0')
		{
			update_existing_var(head, arg, equal_sign_position, arg_len);
			return (EXIT_SUCCESS);
		}
		head = &(*head)->next;
	}
	add_new_var(head, arg, equal_sign_position);
	return (EXIT_SUCCESS);
}

int	create_sorted_env_list(t_env_lst *env_lst)
{
	t_env_lst	*sorted_list;
	t_env_lst	*new_node;

	sorted_list = NULL;
	while (env_lst != NULL)
	{
		new_node = malloc(sizeof(t_env_lst));
		if (new_node == NULL)
			return (EXIT_FAILURE);
		new_node->var_name = ft_strdup(env_lst->var_name);
		if (env_lst->var_value == NULL)
			new_node->var_value = NULL;
		else
			new_node->var_value = ft_strdup(env_lst->var_value);
		new_node->next = NULL;
		sorted_list = insert_sorted(&sorted_list, new_node);
		env_lst = env_lst->next;
	}
	print_export_list(&sorted_list);
	return (EXIT_SUCCESS);
}

/* export_builtin()
 *
 *	- If 'export' is executed with arguments (a new environment variable),
 *		we add the new variable to the env list, or update its value if it
 *		already exists.
 *	- If 'export' is executed without arguments, we create a new linked 
 *		list to store the environment variables SORTED ALPHABETICALLY.
 */
int	export_builtin(t_env_lst *env_lst, token *args_lst)
{
	if (args_lst && ft_list_size(args_lst) > 0)
	{
		while (args_lst != NULL)
		{
			if (set_new_var(&env_lst, args_lst->content) == 1)
				return (EXIT_FAILURE);
			args_lst = args_lst->next;
		}
	}
	else
	{
		if (create_sorted_env_list(env_lst) != 0)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
