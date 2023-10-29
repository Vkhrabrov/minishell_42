/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 20:19:10 by ccarrace          #+#    #+#             */
/*   Updated: 2023/10/29 20:24:15 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
/*
 *	No path provided:	go to user's home directory
 *	
*/

int	cd_builtin(token *args_lst)
{
	char	*path;

	path = args_lst[0].content;
printf("____________path provided = %s____________\n", args_lst[0].content);
	if (path == NULL)
	{
		path = getenv("HOME");
		if (path == NULL)
		{
			printf("HOME environment variable is not set\n");
			return (1);
		}
	}
	if (chdir(path) == 0)
		return (0);
	else
	{
		perror("cd");
		return (1);
	}
}

// int	main(int argc, char **argv)
// {
// 	if (argc == 1)
// 		return cd_builtin(NULL);
// 	else if (argc == 2)
// 		return cd_builtin(argv[1]);
// 	{
// 		ret
// 	}
// }
