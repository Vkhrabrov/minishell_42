/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 13:40:46 by ccarrace          #+#    #+#             */
/*   Updated: 2023/11/07 22:12:40 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Trims single/double quotes and leading spaces
 * If the resulting string starts with a digit, we return the trimmed string
 * But if it starts with a char, we return the original string because we need
 * to include it in the error message
*/
char	*trim_first_arg(char *str)
{
	char	*trimmed_str;
	int		len;

	trimmed_str = str;
	if (*trimmed_str == '"' || *trimmed_str == '\'')
		trimmed_str++;
	len = ft_strlen(trimmed_str);
	while (len > 0 && *trimmed_str == ' ')
	{
		trimmed_str++;
		len--;
	}
	trimmed_str[len] = '\0';
	if (!ft_isdigit(*trimmed_str))
		return (str);
	else
		return (trimmed_str);
}

int	exit_builtin(token *args_lst)
{
	int		exit_status = 0;
	int		num_args;
	size_t	i;
	long	num;
	char	*str;

	// int		len;
	/* _____ ARGS PROVIDED _____ */
	if (args_lst != NULL)
	{
		args_lst->content = trim_first_arg(args_lst->content);
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
					/* _____ SINGLE ARG WITH -- (end of options and beginning of positional args) _____ */
					if (args_lst->content[i] == '-' && !args_lst->content[i + 1])
						printf(("exit status = 0\n"));
					/* _____ SINGLE ARG WITH $ _____ */
					else if (args_lst->content[i] == '$')
						printf("minishell: exit: %s: numeric argument required | exit status = %s\n", args_lst->content, &args_lst->content[i]);
					/* _____ SINGLE ARG NON NUMERIC _____ */
					else
						printf("minishell: exit: %s: numeric argument required | exit status = 255\n", args_lst->content);
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
				{
					/* _____ SINGLE ARG IS OUT OF RANGE _____ */
					printf("argument is %s\n", args_lst->content);
					printf("minishell: exit: %s: numeric argument required | exit status = 255\n", args_lst->content);
				}
				/* _____ SINGLE ARG IS LONG POSITIVE _____ */
				else if (num > 0)
					printf("exit status = %ld\n", num % 256);
				/* _____ SINGLE ARG IS LONG NEGATIVE _____ */
				else if (num < 0)
					printf("exit status = %ld\n", 256 - ((num * -1) % 256));
				/* _____ SINGLE ARG IS '0' _____ */
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
					printf("Numeric argument required | exit status = 255\n");
					break;
				}
				i++;
			}
			/* _____ SEVERAL ARGS, FIRST IS NUMERIC _____ */
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
		exit(exit_status);
		return (0);
	}
}
