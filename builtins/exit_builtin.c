/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 13:40:46 by ccarrace          #+#    #+#             */
/*   Updated: 2023/11/14 01:30:59 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <ctype.h>

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

bool	is_valid_numeric(const char *str)
{
    if (*str == '\0')
        return false;  // Empty string is not a valid numeric value
    // Allow for an optional sign at the beginning
    if (*str == '-' || *str == '+')
        str++;
    while (*str != '\0')
	{
        if (*str < '0' || *str > '9')
            return false;  // Non-numeric character found
        str++;
    }
    return true;  // All characters are valid numeric characters
}

int exit_builtin(token *args_lst)
{
	long	num;
	char	*str;

    if (args_lst == NULL)
        exit(0);
	num = ft_atol(args_lst->content);
	str = ft_ltoa(num);
    if (num == 0 && is_valid_numeric(args_lst->content) == false)
    {
        build_error_msg("1__exit: ", args_lst->content, ": numeric argument required", false);
        exit(255);
    }
    else if (ft_strncmp(args_lst->content, str, find_max_len(args_lst->content, str)) != 0)
    {
        // If the strings don't match, it's out of range
        build_error_msg("2__exit: ", args_lst->content, ": numeric argument required", false);
        exit(255);
    }
    else
    {
        // Handle cases when the number is a long
        if (num > 0)
            exit(num % 256);
        else if (num < 0)
            exit(256 - ((num * -1) % 256));
        else
            exit(0);
    }
}















// int exit_builtin(token *args_lst)
// {
// 	long	num;
// 	char	*str;

//     if (args_lst == NULL)
//         exit(0);
// 	num = ft_atol(args_lst->content);
// 	str = ft_ltoa(num);
//     if (num == 0 && !is_valid_numeric(args_lst->content))
//     {
//         build_error_msg("1__exit: ", args_lst->content, ": numeric argument required", false);
//         exit(255);
//     }
//     else if (ft_strncmp(args_lst->content, str, find_max_len(args_lst->content, str)) != 0)
//     {
//         // If the strings don't match, it's out of range
//         build_error_msg("2__exit: ", args_lst->content, ": numeric argument required", false);
//         exit(255);
//     }
//     else
//     {
//         // Handle cases when the number is a long
//         if (num > 0)
//             exit(num % 256);
//         else if (num < 0)
//             exit(256 - ((num * -1) % 256));
//         else
//             exit(0);
//     }
// }

// int	exit_builtin(token *args_lst)
// {
// 	int		num;
// 	char	*str;

// 	if (args_lst == NULL)
// 		exit(0);
// 	else
// 	{
// 		num = ft_atol(args_lst->content);
// 		if (num == 0  && is_valid_numeric(args_lst->content) == false)
// 		{

// 			// build_error_msg("1.exit: ", args_lst->content, ": numeric argument required", false);
// 			// exit(255);
// 		}
// 		str = ft_ltoa(num);
// 		if (ft_strncmp(args_lst->content, str, find_max_len(args_lst->content, str)) != 0)	// out of long range
// 		{
// 			// write(STDERR_FILENO, "exit\n", 5);
// 			// printf("exit\n");
// 			build_error_msg("2.exit: ", args_lst->content, ": numeric argument required", false);
// 			exit(255);
// 		}
// 		else if (num > 0)	// single arg is long positive
// 		{
// 			// write(STDERR_FILENO, args_lst->content, ft_strlen(args_lst->content));
// 			// write(STDERR_FILENO, "\n", 1);
// 			exit(num % 256);
// 		}
// 		else if (num < 0)	// single arg is long negative
// 		{
// 			// write(STDERR_FILENO, "exit\n", 5);
// 			// printf("exit\n");
// 			exit(256 - ((num * -1) % 256));
// 		}
// 		else				// single arg is zero
// 		{
// 			// write(STDERR_FILENO, "exit\n", 5);
// 			// printf("exit\n");
// 			exit(0);
// 		}		
// 	}
// 	return (0);
// }

// int	exit_builtin(token *args_lst)
// {
// 	int		exit_status;
// 	int		num_args;
// 	size_t	i;
// 	long	num;
// 	char	*str;

// 	exit_status = 0;
// 	// int		len;
// 	/* _____ ARGS PROVIDED _____ */
// 	if (args_lst != NULL)
// 	{
// 		args_lst->content = trim_first_arg(args_lst->content);
// 		num_args = ft_list_size(args_lst);
// 		/* _____ SINGLE ARG _____ */
// 		if (num_args == 1)
// 		{
// 			i = 0;
// 			while (i < ft_strlen(args_lst->content))
// 			{
// 				if (args_lst->content[0] == '-')
// 					i++;
// 				if (!ft_isdigit((int)args_lst->content[i]))
// 				{
// 					/* _____ SINGLE ARG WITH -- (end of options and beginning of positional args) _____ */
// 					if (args_lst->content[i] == '-' && (!args_lst->content[i + 1] || args_lst->content[i + 1] == '-'))
// 						return (0);
// 					else
// 					{
// 						build_error_msg("exit: ", args_lst->content, ": numeric argument required", false);
// 						return (255);
// 					}
// 					break;
// 				}
// 				i++;
// 			}
// 			if (i == ft_strlen(args_lst->content))
// 			{
// 				/* _____ SINGLE ARG NUMERIC _____ */
// 				num = ft_atol(args_lst->content);
// 				str = ft_ltoa(num);		
// 				if (ft_strncmp(args_lst->content, str, find_max_len(args_lst->content, str)) != 0)	// out of long range
// 				{
// 					write(STDERR_FILENO, "exit\n", 5);
// 					build_error_msg("exit: ", args_lst->content, ": numeric argument required", false);
// 					exit(255);
// 				}
// 				else if (num > 0)	// single arg is long positive
// 				{
// 					write(STDERR_FILENO, args_lst->content, ft_strlen(args_lst->content));
// 					write(STDERR_FILENO, "\n", 1);
// 					exit(num % 256);
// 				}
// 				else if (num < 0)	// single arg is long negative
// 				{
// 					write(STDERR_FILENO, "exit\n", 5);
// 					exit(256 - ((num * -1) % 256));
// 				}
// 				else				// single arg is zero
// 				{
// 					write(STDERR_FILENO, "exit\n", 5);
// 					exit(0);
// 				}
// 			}	
// 		}
// 		else
// 		/* _____ SEVERAL ARGS _____ */
// 		{
// 			i = 0;
// 			while (i < ft_strlen(args_lst->content))
// 			{
// 				/* _____ SEVERAL ARGS, FIRST IS NOT NUMERIC _____*/
// 				if (!ft_isdigit((int)args_lst->content[i]))
// 				{
// 					printf("Numeric argument required | exit status = 255\n");
// 					break;
// 				}
// 				i++;
// 			}
// 			/* _____ SEVERAL ARGS, FIRST IS NUMERIC _____ */
// 			if (i == ft_strlen(args_lst->content))
// 				printf("Too many arguments | exit status = 1\n");
// 		}
// 		// exit_status = (ft_atol(args_lst->content));
// 		// return (exit_status);
// 		return (0);
// 	}
// 	else
// 	/* _____ NO ARGS PROVIDED _____ */
// 	{
// 		printf("exit\n");
// 		exit(exit_status);
// 		return (0);
// 	}
// }

/* 
 * _______________________________ EXIT TREE ______________________________
 */
// int	exit_builtin(token *args_lst)
// {
// 	int		exit_status;
// 	int		num_args;
// 	size_t	i;
// 	long	num;
// 	char	*str;

// 	exit_status = 0;
// 	// int		len;
// 	/* _____ ARGS PROVIDED _____ */
// 	if (args_lst != NULL)
// 	{
// 		args_lst->content = trim_first_arg(args_lst->content);
// 		num_args = ft_list_size(args_lst);
// 		/* _____ SINGLE ARG _____ */
// 		if (num_args == 1)
// 		{
// 			i = 0;
// 			while (i < ft_strlen(args_lst->content))
// 			{
// 				if (args_lst->content[0] == '-')
// 					i++;
// 				if (!ft_isdigit((int)args_lst->content[i]))
// 				{
// 					/* _____ SINGLE ARG WITH -- (end of options and beginning of positional args) _____ */
// 					if (args_lst->content[i] == '-' && !args_lst->content[i + 1])
// 						printf(("exit status = 0\n"));
// 					/* _____ SINGLE ARG WITH $ _____ */
// 					else if (args_lst->content[i] == '$')
// 						printf("minishell: exit: %s: numeric argument required | exit status = %s\n", args_lst->content, &args_lst->content[i]);
// 					/* _____ SINGLE ARG NON NUMERIC _____ */
// 					else
// 						printf("minishell: exit: %s: numeric argument required | exit status = 255\n", args_lst->content);
// 					break;
// 					// perror("exit");
// 				}
// 				i++;
// 			}
// 			if (i == ft_strlen(args_lst->content))
// 			{
// 				/* _____ SINGLE ARG NUMERIC _____ */
// 				num = ft_atol(args_lst->content);
// 				str = ft_ltoa(num);		
// 				if (ft_strncmp(args_lst->content, str, find_max_len(args_lst->content, str)) != 0)
// 				{
// 					/* _____ SINGLE ARG IS OUT OF RANGE _____ */
// 					printf("argument is %s\n", args_lst->content);
// 					printf("minishell: exit: %s: numeric argument required | exit status = 255\n", args_lst->content);
// 				}
// 				/* _____ SINGLE ARG IS LONG POSITIVE _____ */
// 				else if (num > 0)
// 					printf("exit status = %ld\n", num % 256);
// 				/* _____ SINGLE ARG IS LONG NEGATIVE _____ */
// 				else if (num < 0)
// 					printf("exit status = %ld\n", 256 - ((num * -1) % 256));
// 				/* _____ SINGLE ARG IS '0' _____ */
// 				else
// 					printf("exit status = 0\n");
// 			}	
// 		}
// 		else
// 		/* _____ SEVERAL ARGS _____ */
// 		{
// 			i = 0;
// 			while (i < ft_strlen(args_lst->content))
// 			{
// 				/* _____ SEVERAL ARGS, FIRST IS NOT NUMERIC _____*/
// 				if (!ft_isdigit((int)args_lst->content[i]))
// 				{
// 					printf("Numeric argument required | exit status = 255\n");
// 					break;
// 				}
// 				i++;
// 			}
// 			/* _____ SEVERAL ARGS, FIRST IS NUMERIC _____ */
// 			if (i == ft_strlen(args_lst->content))
// 				printf("Too many arguments | exit status = 1\n");
// 		}
// 		// exit_status = (ft_atol(args_lst->content));
// 		// return (exit_status);
// 		return (0);
// 	}
// 	else
// 	/* _____ NO ARGS PROVIDED _____ */
// 	{
// 		printf("exit\n");
// 		exit(exit_status);
// 		return (0);
// 	}
// }
