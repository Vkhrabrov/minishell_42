/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 19:20:45 by ccarrace          #+#    #+#             */
/*   Updated: 2023/10/27 23:45:50 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int pwd_builtin(token *args_lst)
{
    char 	buffer[PATH_MAX];
    char	*curr_work_dir;
	char	*semicolon_position;
  
	curr_work_dir = getcwd(buffer, sizeof(buffer));
    if (curr_work_dir)
        printf("%s\n", buffer);
    else
        perror("getcwd");
	while (args_lst != NULL)
	{
		semicolon_position = ft_strchr(args_lst->content, ';');
		if (semicolon_position != NULL)
			printf("minishell: %s: command not found\n", semicolon_position + 1);
		args_lst = args_lst->next;
	}
	return (0);
}

// int pwd_builtin(token *args_lst)
// {
//     char 	buffer[PATH_MAX];
//     char	*curr_work_dir;
  
// 	curr_work_dir = getcwd(buffer, sizeof(buffer));
//     if (curr_work_dir)
//         printf("%s\n", buffer);
//     else
//         perror("getcwd");
//     return 0;
// }

// int	main(void)
// {
// 	pwd_builtin();
// 	return (0);
// }
