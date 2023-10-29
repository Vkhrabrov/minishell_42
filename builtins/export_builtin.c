/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 21:00:11 by ccarrace          #+#    #+#             */
/*   Updated: 2023/10/29 23:13:10 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int export_builtin(t_env_lst *env_lst)
{
    t_env_lst	*sorted;
	t_env_lst	*current;
	t_env_lst	*temp;
	size_t		len1;
	size_t		len2;
	size_t		max_len;

    if (env_lst == NULL || env_lst->next == NULL)
        return (1); // Already sorted or empty list
    sorted = NULL;
    while (env_lst != NULL)
	{
        current = env_lst;
        env_lst = env_lst->next;

		len1 = ft_strlen(current->var_name);
		if (sorted != NULL)
			len2 = ft_strlen(sorted->var_name);
		if (len1 > len2)
			max_len = len1;
		else 
			max_len = len2;

        if (sorted == NULL || ft_strncmp(current->var_name, sorted->var_name, max_len) >= 0)
		{
            current->next = sorted;
            sorted = current;
        }
		else
		{
            temp = sorted;
            while (temp->next != NULL && ft_strncmp(current->var_name, temp->next->var_name, max_len) < 0)
                temp = temp->next;
            current->next = temp->next;
            temp->next = current;
        }
    }
	print_env_list(&sorted);
    // env_lst = sorted; // Update the head to point to the sorted list
	return (0);
}

// int	export_builtin(token *args_lst)
// {
// 	t_env_lst	*current = 
// 	while (args_lst != NULL)
// 	{
// 		if (args_lst->value[0] > args_lst->next->value[0])
// 		args_lst = args_lst->next;
// 	}
//     // Only populate the list on the first call
//     if (env_lst == NULL)
//         fill_env_list(&env_lst, envp);
//     if (argc > 1)
// 	{
//         i = 1;
//         // fill_env_list(&env_lst, envp);
//         while (i < argc)
// 		{
//             add_env_var_to_list(&env_lst, argv[i]);
//             i++;
//         }
//     }
//     print_env_lst(&env_lst);
//     return 0;
// }
