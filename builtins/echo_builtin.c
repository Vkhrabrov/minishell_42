/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 22:11:13 by ccarrace          #+#    #+#             */
/*   Updated: 2023/10/24 23:47:07 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// gcc echo_builtin.c ../libft/ft_strncmp.c -I ../libft -o echo

// #include "minishell.h"
#include <stdio.h>
#include <string.h>		//	strncmp() --> replace with ft_strcnmp()!!!!!
#include "../libft/libft.h"

void	echo_builtin(int argc, char *argv[])
{
	int	i;
	int	option_flag;

	if (argc == 1)
		printf("\n");
	else
	{
		i = 1;
		option_flag = 0;
		while (i < argc)
		{
			if (strncmp(argv[1], "-n", 1) == 0)
				option_flag = 1;
			else
			{
				printf("%s ", argv[i]);
				i++;
			}
		}
		if (option_flag == 0)
			printf("\n");
	}
}

int	main(int argc, char **argv)
{
	echo_builtin(argc, argv);
	return (0);
}
