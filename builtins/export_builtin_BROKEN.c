/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin_BROKEN.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 21:00:11 by ccarrace          #+#    #+#             */
/*   Updated: 2023/12/01 00:03:33 by ccarrace         ###   ########.fr       */
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
static bool	is_var_name_valid(char *arg, int equal_sign_position)
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

/* update_existing_node()
 *
 *	- Searches for an existing node with the same name as provided in arg
 *	- If a node with the same name exists, updates its value
 *		-- If no value is provided in arg, sets it to NULL
 *	- If the node is already updated, exits
 */
static int update_existing_node(t_env_lst **head, char *arg, size_t equal_sign_position, size_t arg_len)
{
    while (*head != NULL) 
	{
        if (ft_strncmp((*head)->var_name, arg, equal_sign_position) == 0
            && (*head)->var_name[equal_sign_position] == '\0')
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
            return (EXIT_SUCCESS);
        }
        head = &(*head)->next;
    }
    return (EXIT_FAILURE);
}

/* create_new_node()
 *	(is set_new_var part 1/3)
 *	- If no node exists with the same name as provided in arg, creates it
 */
static int create_new_node(t_env_lst **head, char *arg, size_t equal_sign_position)
{
    t_env_lst *new_node = malloc(sizeof(t_env_lst));
    if (new_node == NULL)
        return (EXIT_FAILURE);

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
    return (EXIT_SUCCESS);
}

static int set_new_var(t_env_lst **head, char *arg)
{
    size_t equal_sign_position = find_char_index(arg, '=');
    size_t arg_len = ft_strlen(arg);

    if (is_var_name_valid(arg, equal_sign_position) == false)
        return (build_error_msg("export: ", arg, MS_INVALIDID, true));
    if (update_existing_node(head, arg, equal_sign_position, arg_len) == 0)
        return (0);
    return (create_new_node(head, arg, equal_sign_position));
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

int	create_sorted_env_list(t_env_lst *env_lst)
{
   	t_env_lst	*sorted_list;
	t_env_lst 	*new_node;

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
int export_builtin(t_env_lst *env_lst, token *args_lst)
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
        // // Create a new linked list to store the sorted environment variables
        // sorted_list = NULL;
        // // Copy all of the environment variables to the new linked list
        // while (env_lst != NULL) {
        //     new_node = malloc(sizeof(t_env_lst));
        //     if (new_node == NULL)
        //         return (EXIT_FAILURE);
        //     new_node->var_name = ft_strdup(env_lst->var_name);
		// 	if (env_lst->var_value == NULL)
		// 		new_node->var_value = NULL;
		// 	else
        //     	new_node->var_value = ft_strdup(env_lst->var_value);
			
        //     new_node->next = NULL;
        //     sorted_list = insert_sorted(&sorted_list, new_node);
        //     env_lst = env_lst->next;
        // }
		if (create_sorted_env_list(env_lst) != 0)
			return (EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}

