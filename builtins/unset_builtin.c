/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 20:29:41 by ccarrace          #+#    #+#             */
/*   Updated: 2023/11/26 14:17:46 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * We pass a pointer to the pointer of the head of the list because we may need 
 * to modify the head of the list if the first node matches. If we only pass a 
 * pointer to the list (t_env_lst *env_lst), we won't be able to update the 
 * head of the list if we need to remove the first node. 
*/
int unset_builtin(t_env_lst **env_lst, token *args_lst)
{
    t_env_lst *current;
    t_env_lst *previous;
    int var_name_len;

    var_name_len = ft_strlen(args_lst->content);
    while (args_lst != NULL)
    {
        current = *env_lst;
        previous = NULL;
        while (current != NULL)
        {
            if (ft_strncmp(args_lst->content, current->var_name, var_name_len) == 0)
            {
                if (previous == NULL)
                    *env_lst = current->next; // Update the head if the first node matches
                else
                    previous->next = current->next;
                free(current->var_name);
                free(current->var_value);
                free(current);
                current = NULL; // Reset current to avoid accessing freed memory
				current = *env_lst;
            }
            else
            {
                previous = current;
                current = current->next;
            }
        }
        args_lst = args_lst->next;
    }
    return (0);
}

