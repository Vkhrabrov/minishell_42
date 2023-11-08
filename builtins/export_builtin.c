/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 21:00:11 by ccarrace          #+#    #+#             */
/*   Updated: 2023/11/08 22:50:49 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

static bool	is_var_name_valid(char *arg, int equal_sign_position)
{
	int	i;

	i = 0;
	while (i < equal_sign_position)
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

static int set_new_var(t_env_lst **head, char *arg)
{
    size_t 	equal_sign_position = find_char_index(arg, '=');
    size_t 	arg_len = ft_strlen(arg);

	if (is_var_name_valid(arg, equal_sign_position) == false)
	{
		build_error_msg("export: ", arg, " : not a valid identifier", true);
		return (1);
	}
    // Search for an existing node with the same name
    while (*head != NULL)
	{
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
			else
			{
                free((*head)->var_value);
                (*head)->var_value = ft_strdup(arg + equal_sign_position + 1);
            }
            return (0); // Node already updated, no need to create a new one
        }
        head = &(*head)->next;
    }
    // If no existing node with the same name, create a new node
    t_env_lst *new_node = malloc(sizeof(t_env_lst));
    if (new_node == NULL)
        return (1);
    if (equal_sign_position == arg_len)
	{
        new_node->var_name = ft_strdup(arg);
        new_node->var_value = NULL;
    }
	else
	{
        new_node->var_name = ft_substr(arg, 0, equal_sign_position);
        new_node->var_value = ft_strdup(arg + equal_sign_position + 1);
    }
    // new_node->next = NULL;
	new_node->next = *head;
    *head = new_node;
	return (0);
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

