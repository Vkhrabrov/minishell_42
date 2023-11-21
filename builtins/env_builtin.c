/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 21:00:11 by ccarrace          #+#    #+#             */
/*   Updated: 2023/11/20 23:03:49 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
    'envp' is derived from "environment pointer".
    The 'envp' argument is automatically provided by the operating system.
*/
int	env_builtin(t_env_lst *env_lst, token *args_lst)
{
	t_env_lst *current;
	
	if (ft_list_size(args_lst) > 0)
		return(build_error_msg("env: ", args_lst->content, ": No such file or directory", false));		
	else
	{
		current = env_lst;
		while (current != NULL)
		{
			if (current->var_value != NULL)
				printf("%s=%s\n", current->var_name, current->var_value);
			current = current->next;     
		}
	}
	return (0);
}

// int env_builtin(t_env_lst *env_lst, token *search_var)
// {
//     t_env_lst *current = env_lst;

//     if (search_var->content == NULL)
// 	{
//         while (current != NULL)
// 		{
//             if (current->var_value != NULL)
//                 printf("%s=%s\n", current->var_name, current->var_value);
//             current = current->next;
//         }
//         return 0; // Return success
//     }
// 	else
// 	{
//         while (current != NULL)
// 		{
//             if (current->var_name != NULL && ft_strncmp(current->var_name, search_var->content, find_max_len(current->var_name, search_var->content)) == 0)
// 			{
//                 if (current->var_value != NULL)
// 				{
//                     printf("%s=%s\n", current->var_name, current->var_value);
//                     return 0; // Variable found, return success
//                 }
// 				else
//                     return 1; // Variable found, but value is NULL, return failure
//             }
//             current = current->next;
//         }
//         return 1; // Variable not found, return failure
//     }
// }

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
