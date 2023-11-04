/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 21:00:11 by ccarrace          #+#    #+#             */
/*   Updated: 2023/11/03 23:45:55 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
    'envp' is derived from "environment pointer".
    The 'envp' argument is automatically provided by the operating system.
*/

int	env_builtin(t_env_lst *env_lst)
{
	t_env_lst *current;
	
	current = env_lst;
    while (current != NULL)
    {
		if (current->var_value != NULL)
        	printf("%s=%s\n", current->var_name, current->var_value);
        current = current->next;     
    }
	return (0);
}

// void env(char *envp[]) 
// {
//     int i;

//     i = 0;
//     while (envp[i] != NULL)
//     {
//         printf("%s\n", envp[i]); 
//         i++;       
//     }
// }

// int main(int argc, char *argv[], char *envp[]) 
// {
//     (void)argc;
//     (void)argv;

//     env(envp);
//     return (0);
// }
