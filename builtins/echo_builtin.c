/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 22:11:13 by ccarrace          #+#    #+#             */
/*   Updated: 2023/10/25 20:05:57 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// gcc echo_builtin.c ../libft/ft_strncmp.c -I ../libft -o echo

// #include "minishell.h"
#include <stdio.h>
#include <string.h>		//	strncmp(), strlen() --> REPLACE with libf fts!!!!!
#include <stdbool.h>
#include "../libft/libft.h"


void echo_builtin(int argc, char *argv[])
{
    int i;
    bool option_passed;

	option_passed = false;
    if (argc == 1)
        printf("\n");
    else {
        if (strncmp(argv[1], "-n", 2) == 0 && strlen(argv[1]) == 2)
            option_passed = true;
        if (option_passed == true)
    		i = 2;
		else
    		i = 1;
        while (i < argc)
		{
            printf("%s", argv[i]);
            if (i < argc - 1)
                printf(" ");
            i++;
        }
        if (option_passed == false)
            printf("\n");
    }
}

int	main(int argc, char **argv)
{
	echo_builtin(argc, argv);
	return (0);
}
