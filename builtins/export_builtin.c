/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 21:00:11 by ccarrace          #+#    #+#             */
/*   Updated: 2023/10/31 00:07:23 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_env_lst *copy_env_list(t_env_lst *env_lst)
{
	t_env_lst	*head;
	t_env_lst	*tail;
	t_env_lst	*new_node;

	// head = NULL;
	tail = NULL;
	while (env_lst != NULL)
	{
		new_node = malloc(sizeof(t_env_lst));
		if (!new_node)
			return (NULL);
		new_node->var_name = ft_strdup(env_lst->var_name);
		new_node->var_value = ft_strdup(env_lst->var_value);
		new_node->next = NULL;
		if (tail == NULL)
		{
			head = new_node;
			tail = new_node;
		}
		else
		{
			tail->next = new_node;
			tail = new_node;
		}
		env_lst = env_lst->next;
	}
	return (head);
}

void	print_export_list(t_env_lst **env_lst)
{
	t_env_lst *current;
	
	current = *env_lst;
    while (current != NULL)
    {
        printf("declare -x %s", current->var_name);
        printf("=\"%s", current->var_value);
        printf("\"\n");
        current = current->next;       
    }
}

void	free_env_lst_copy(t_env_lst *env_lst_copy)
{
    t_env_lst *temp;

    while (env_lst_copy != NULL) {
        temp = env_lst_copy;
        env_lst_copy = env_lst_copy->next;
        free(temp->var_name);
		free(temp->var_value);
        free(temp);
    }
}

int	ft_list_size(token *args_lst)
{
	int	i;

	i = 0;
	while (args_lst)
	{
		i++;
		args_lst = args_lst->next;
	}
	return (i);
}

static void set_new_var(t_env_lst **head, char *arg)
{
    t_env_lst *new_node;
    size_t equal_sign_position;
    size_t arg_len;

    new_node = malloc(sizeof(t_env_lst));
    if (new_node == NULL)
		return ;
    equal_sign_position = find_char_index(arg, '=');
    arg_len = ft_strlen(arg);
    if (equal_sign_position == arg_len)
	{
        new_node->var_name = ft_strdup(arg);
        new_node->var_value = NULL;
    }
	else
	{
        new_node->var_name = ft_substr(arg, 0, equal_sign_position);
        new_node->var_value = ft_substr(arg, equal_sign_position + 1, arg_len - equal_sign_position);
    }
	while (*head != NULL)
		head = &(*head)->next;
	*head = new_node;
    // insert_sorted(head, new_node);
}

int export_builtin(t_env_lst *env_lst, token *args_lst)
{
    t_env_lst	*sorted;
	t_env_lst	*current;
	t_env_lst	*temp;
	size_t		len1;
	size_t		len2;
	size_t		max_len;
	t_env_lst	*env_lst_copy; 
	
	if (ft_list_size(args_lst) > 0)
	{
		while (args_lst != NULL)
		{
			set_new_var(&env_lst, args_lst->content);
			args_lst = args_lst->next;
		}
	}
	else
	{
		env_lst_copy = copy_env_list(env_lst);
		if (env_lst_copy == NULL || env_lst_copy->next == NULL)
			return (1); // Already sorted or empty list
		sorted = NULL;
		while (env_lst_copy != NULL)
		{
			current = env_lst_copy;
			env_lst_copy = env_lst_copy->next;
			len1 = ft_strlen(current->var_name);
			if (sorted != NULL)
				len2 = ft_strlen(sorted->var_name);
			if (len1 > len2)
				max_len = len1;
			else 
				max_len = len2;
			if (sorted == NULL || ft_strncmp(current->var_name, sorted->var_name, max_len) <= 0)
			{
				current->next = sorted;
				sorted = current;
			}
			else
			{
				temp = sorted;
				while (temp->next != NULL && ft_strncmp(current->var_name, temp->next->var_name, max_len) > 0)
					temp = temp->next;
				current->next = temp->next;
				temp->next = current;
			}
		}
		print_export_list(&sorted);
		free_env_lst_copy(env_lst_copy);
	}
	return (0);
}
