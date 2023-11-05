/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 11:36:02 by ccarrace          #+#    #+#             */
/*   Updated: 2023/11/05 11:57:39 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_args_list(token *args_lst)
{
    token *current;

    while (args_lst != NULL)
	{
        current = args_lst;
        free(current->content);
		free(current);
        args_lst = args_lst->next;
    }	
}