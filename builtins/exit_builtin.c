/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 13:40:46 by ccarrace          #+#    #+#             */
/*   Updated: 2023/11/07 01:24:59 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	exit_builtin(token *args_lst)
{
	// int		exit_status = 0;
	int		num_args;
	size_t	i;
	long	num;
	char	*str;

	/* _____ ARGS PROVIDED _____ */
	if (args_lst != NULL)
	{
		num_args = ft_list_size(args_lst);
		/* _____ SINGLE ARG _____ */
		if (num_args == 1)
		{
			i = 0;
			while (i < ft_strlen(args_lst->content))
			{
				if (args_lst->content[0] == '-')
					i++;
				if (!ft_isdigit((int)args_lst->content[i]))
				{
					/* _____ SINGLE ARG WITH $ _____ */
					if (args_lst->content[i] == '$')
						printf("A-Numeric argument required | exit status = %s\n", &args_lst->content[i]);
					/* _____ SINGLE ARG NON NUMERIC _____ */
					else
						printf("B-Numeric argument required | exit status = 255\n");
					break;
					// perror("exit");
				}
				i++;
			}
			if (i == ft_strlen(args_lst->content))
			{
				/* _____ SINGLE ARG NUMERIC _____ */
				num = ft_atol(args_lst->content);
				str = ft_ltoa(num);		
				if (ft_strncmp(args_lst->content, str, find_max_len(args_lst->content, str)) != 0)
					/* _____ SINGLE ARG IS OUT OF RANGE _____ */
					printf("C-Numeric argument required | exit status = 255\n");
				else if (num > 0)
					printf("exit status = %ld\n", num % 256);
				else if (num < 0)
					printf("exit status = %ld\n", 256 - ((num * -1) % 256));
				else
					printf("exit status = 0\n");
			}	
		}
		else
		/* _____ SEVERAL ARGS _____ */
		{
			i = 0;
			while (i < ft_strlen(args_lst->content))
			{
				/* _____ SEVERAL ARGS, FIRST IS NOT NUMERIC _____*/
				if (!ft_isdigit((int)args_lst->content[i]))
				{
					printf("Too many arguments | exit status = 255\n");
					break;
				}
				i++;
			}
			/* _____ SEVERAL ARGS, FIRS IS NUMERIC _____ */
			if (i == ft_strlen(args_lst->content))
				printf("Too many arguments | exit status = 1\n");
		}
		// exit_status = (ft_atol(args_lst->content));
		// return (exit_status);
		return (0);
	}
	else
	/* _____ NO ARGS PROVIDED _____ */
	{
		printf("exit\n");
		// exit(exit_status);
		return (0);
	}
}
