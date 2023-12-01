/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 21:00:11 by ccarrace          #+#    #+#             */
/*   Updated: 2023/12/01 01:15:10 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//ESTA VERSION AUN NO SEPARA FUNCIONES, PERO APARENTEMENTE EL PROBLEMA DE IMPRESION
// DE VARIABLES SIN VALOR SE HA SOLUCIONADO
// export A1 ya guarda la variable y la muestra en la lista
// (el problema estaba en set_new_var, rama crear nueva variable, la igualdad
// (equal_sign_position == arg_len) no se cumple con variables sin valor porque
// equal_sign_position es 0)

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




static void	update_existing_node(t_env_lst **head, char *arg, size_t equal_sign_position, size_t arg_len)
{
    while (*head != NULL)
	{
        if (ft_strncmp((*head)->var_name, arg, equal_sign_position) == 0 &&
            (*head)->var_name[equal_sign_position] == '\0')
			{
            // Node with the same name exists, update its value
            if (equal_sign_position == arg_len)
			{
printf("Found coincidence: %s | %s\n", (*head)->var_name, arg);
                // If no value provided in arg, set it to NULL
                if ((*head)->var_value != NULL)
				{
printf("arg has no value, %s will be set to NULL\n", arg);
                    free((*head)->var_value);
                    (*head)->var_value = NULL;
                }
            }
			else
			{
                free((*head)->var_value);
                (*head)->var_value = ft_strdup(arg + equal_sign_position + 1);
            }
            // return (0); // Node already updated, no need to create a new one
        }
        head = &(*head)->next;
    }
	// return (1);	
}





static int set_new_var(t_env_lst **head, char *arg)
{
    size_t 	equal_sign_position = find_char_index(arg, '=');
    size_t 	arg_len = ft_strlen(arg);
	int		exit_status;

	exit_status = 0;
	if (is_var_name_valid(arg, equal_sign_position) == false)
		return (build_error_msg("export: ", arg, ": not a valid identifier", true));
    // Search for an existing node with the same name
    // while (*head != NULL)
	// {
    //     if (ft_strncmp((*head)->var_name, arg, equal_sign_position) == 0 &&
    //         (*head)->var_name[equal_sign_position] == '\0')
	// 		{
    //         // Node with the same name exists, update its value
    //         if (equal_sign_position == arg_len)
	// 		{
    //             // If no value provided in arg, set it to NULL
    //             if ((*head)->var_value != NULL)
	// 			{
    //                 free((*head)->var_value);
    //                 (*head)->var_value = NULL;
    //             }
    //         }
	// 		else
	// 		{
    //             free((*head)->var_value);
    //             (*head)->var_value = ft_strdup(arg + equal_sign_position + 1);
    //         }
    //         return (0); // Node already updated, no need to create a new one
    //     }
    //     head = &(*head)->next;
    // }
    // If no existing node with the same name, create a new node
	if (equal_sign_position != 0)
	{
		update_existing_node(head, arg, equal_sign_position, arg_len);
printf("equal_sign_position = %zu -> %s: node already exists\n", equal_sign_position, arg);
    } 
	else
	{
printf("equal_sign_position = %zu -> %s: node does not exist\n", equal_sign_position, arg);		
		t_env_lst *new_node = malloc(sizeof(t_env_lst));
		if (new_node == NULL)
			return (1);
		if (equal_sign_position == 0)
		{
printf("%s: arg comes with no value, it will be set to NULL\n", arg);
			new_node->var_name = ft_strdup(arg);
			new_node->var_value = NULL;
		}
		else
		{
printf("%s: arg has a value, new var with value should be created\n", arg);
			new_node->var_name = ft_substr(arg, 0, equal_sign_position);
			new_node->var_value = ft_strdup(arg + equal_sign_position + 1);
		}
		new_node->next = *head;
		*head = new_node;
	}
	return (0);
}

t_env_lst *insert_sorted(t_env_lst **head, t_env_lst *new_node)
{
    t_env_lst *current = *head;
    t_env_lst *prev = NULL;

	while (current != NULL && ft_strncmp(new_node->var_name, current->var_name, strlen(current->var_name)) > 0)
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
		if (create_sorted_env_list(env_lst) != 0)
			return (EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}
