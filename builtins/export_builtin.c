/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 21:00:11 by ccarrace          #+#    #+#             */
/*   Updated: 2023/11/03 23:17:34 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// t_env_lst *copy_env_list(t_env_lst *env_lst)
// {
// 	t_env_lst	*head;
// 	t_env_lst	*tail;
// 	t_env_lst	*new_node;

// 	// head = NULL;
// 	tail = NULL;
// 	while (env_lst != NULL)
// 	{
// 		new_node = malloc(sizeof(t_env_lst));
// 		if (!new_node)
// 			return (NULL);
// 		new_node->var_name = ft_strdup(env_lst->var_name);
// 		new_node->var_value = ft_strdup(env_lst->var_value);
// 		new_node->next = NULL;
// 		if (tail == NULL)
// 		{
// 			head = new_node;
// 			tail = new_node;
// 		}
// 		else
// 		{
// 			tail->next = new_node;
// 			tail = new_node;
// 		}
// 		env_lst = env_lst->next;
// 	}
// 	return (head);
// }

void	print_export_list(t_env_lst **env_lst)
{
	t_env_lst *current;
	
	current = *env_lst;
    while (current != NULL)
    {
        printf("declare -x %s", current->var_name);
		// if (current->var_value != NULL)
        // 	printf("=\"%s", current->var_value);
        // printf("\"\n");
		if (current->var_value != NULL)
        	printf("=\"%s\"\n", current->var_value);
		else
        	printf("\n");		
        current = current->next;       
    }
}

// void	free_env_lst_copy(t_env_lst *env_lst_copy)
// {
//     t_env_lst *temp;

//     while (env_lst_copy != NULL)
// 	{
//         temp = env_lst_copy;
//         env_lst_copy = env_lst_copy->next;
//         free(temp->var_name);
// 		if (temp->var_value != NULL)
// 			free(temp->var_value);
//         free(temp);
//     }
// }

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
    size_t equal_sign_position = find_char_index(arg, '=');
    size_t arg_len = ft_strlen(arg);

    // Search for an existing node with the same name
    while (*head != NULL) {
        if (ft_strncmp((*head)->var_name, arg, equal_sign_position) == 0 &&
            (*head)->var_name[equal_sign_position] == '\0')
			{
            // Node with the same name exists, update its value
            if (equal_sign_position == arg_len)
			{
                // If no value provided in arg, set it to NULL
                if ((*head)->var_value != NULL)
				{
                    free((*head)->var_value);
                    (*head)->var_value = NULL;
                }
            }
			else if (ft_strncmp((*head)->var_value, arg + equal_sign_position + 1, arg_len - equal_sign_position - 1) != 0) {
                // Update value if it's different
                free((*head)->var_value);
                (*head)->var_value = ft_strdup(arg + equal_sign_position + 1);
            }
            return; // Node already updated, no need to create a new one
        }
        head = &(*head)->next;
    }
    // If no existing node with the same name, create a new node
    t_env_lst *new_node = malloc(sizeof(t_env_lst));
    if (new_node == NULL)
        return;

    if (equal_sign_position == arg_len) {
        new_node->var_name = ft_strdup(arg);
        new_node->var_value = NULL;
    } else {
        new_node->var_name = ft_substr(arg, 0, equal_sign_position);
        new_node->var_value = ft_strdup(arg + equal_sign_position + 1);
    }
    // new_node->next = NULL;
	new_node->next = *head;
    *head = new_node;
}

t_env_lst *insert_sorted(t_env_lst **head, t_env_lst *new_node)
{
    t_env_lst *current = *head;
    t_env_lst *prev = NULL;

    // it doesn't matter whether the length we pass to ft_strncomp is 
	// 'strlen(current->var_name)' or 'strlen(new_node->var_name)'
	while (current != NULL && ft_strncmp(new_node->var_name, current->var_name, strlen(current->var_name)) > 0)
	{
        prev = current;
        current = current->next;
    }
    if (prev == NULL)
	{
        // Insert at the beginning
        new_node->next = *head;
        *head = new_node;
    }
	else
	{
        // Insert in the middle or at the end
        prev->next = new_node;
        new_node->next = current;
    }
	return (*head);
}

int export_builtin(t_env_lst *env_lst, token *args_lst)
{
    t_env_lst	*sorted_list;
	t_env_lst *new_node;

    if (args_lst && ft_list_size(args_lst) > 0)
	{
        while (args_lst != NULL)
		{
            set_new_var(&env_lst, args_lst->content);
            args_lst = args_lst->next;
        }
        // print_export_list(&env_lst);
    }
	else
	{
        // Create a new linked list to store the sorted environment variables
        sorted_list = NULL;
        // Copy all of the environment variables to the new linked list
        while (env_lst != NULL) {
            new_node = malloc(sizeof(t_env_lst));
            if (new_node == NULL)
                return (1);
            new_node->var_name = ft_strdup(env_lst->var_name);

			if (env_lst->var_value == NULL)
				new_node->var_value = NULL;
			else
            	new_node->var_value = ft_strdup(env_lst->var_value);
			
            new_node->next = NULL;
            sorted_list = insert_sorted(&sorted_list, new_node);
            env_lst = env_lst->next;
        }
        // Print the sorted list of environment variables
        print_export_list(&sorted_list);
        // Free the sorted list of environment variables
        // while (sorted_list != NULL)
		// {
        //     t_env_lst *next_node = sorted_list->next;
        //     free(sorted_list->var_name);
		// 	if (sorted_list->var_value != NULL)
        //     	free(sorted_list->var_value);
        //     free(sorted_list);
        //     sorted_list = next_node;
        // }
    }
    return (0);
}

// int export_builtin(t_env_lst *env_lst, token *args_lst)
// {
//     t_env_lst	*sorted;
	
// 	if (ft_list_size(args_lst) > 0)
// 	{
// 		while (args_lst != NULL) 
// 		{
// 			set_new_var(&env_lst, args_lst->content);
// 			args_lst = args_lst->next;
// 		}
// 	}
// 	else
// 	{
// 		sorted = NULL;
// 		while (env_lst != NULL)
// 		{
// 			insert_sorted(&sorted, env_lst);
// 			env_lst = env_lst->next;
// 		}
// 		print_export_list(&sorted);
// 	}
// 	return (0);
// }

// int export_builtin(t_env_lst *env_lst, token *args_lst)
// {
//     t_env_lst	*sorted;
// 	t_env_lst	*current;
// 	t_env_lst	*temp;
// 	size_t		len1;
// 	size_t		len2;
// 	size_t		max_len;
// 	t_env_lst	*env_lst_copy; 
	
// 	if (ft_list_size(args_lst) > 0)
// 	{
// 		while (args_lst != NULL) 
// 		{
// 			set_new_var(&env_lst, args_lst->content);
// 			args_lst = args_lst->next;
// 		}
// 	}
// 	else
// 	{
// 		env_lst_copy = copy_env_list(env_lst);
// 		if (env_lst_copy == NULL || env_lst_copy->next == NULL)
// 			return (1); // Already sorted or empty list
// 		sorted = NULL;
// 		while (env_lst_copy != NULL)
// 		{
// 			current = env_lst_copy;
// 			env_lst_copy = env_lst_copy->next;
// 			len1 = ft_strlen(current->var_name);
// 			if (sorted != NULL)
// 				len2 = ft_strlen(sorted->var_name);
// 			if (len1 > len2)
// 				max_len = len1;
// 			else 
// 				max_len = len2;
// 			if (sorted == NULL || ft_strncmp(current->var_name, sorted->var_name, max_len) <= 0)
// 			{
// 				current->next = sorted;
// 				sorted = current;
// 			}
// 			else
// 			{
// 				temp = sorted;
// 				while (temp->next != NULL && ft_strncmp(current->var_name, temp->next->var_name, max_len) > 0)
// 					temp = temp->next;
// 				current->next = temp->next;
// 				temp->next = current;
// 			}
// 		}
// 		print_export_list(&sorted);
// 		free_env_lst_copy(env_lst_copy);
// 	}
// 	return (0);
// }

