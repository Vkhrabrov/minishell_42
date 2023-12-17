/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 22:11:13 by ccarrace          #+#    #+#             */
/*   Updated: 2023/12/17 22:15:22 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* process_options()
 *
 *	Checks if option '-n' passed is valid or not
 */
bool	process_options(token **args_lst)
{
	bool	is_valid_option;

	is_valid_option = false;
	while (*args_lst != NULL && ft_strncmp((*args_lst)->content, "-n", 2) == 0)
	{
		if ((*args_lst)->content[2] && (*args_lst)->content[2] != 'n')
			break ;
		else
			is_valid_option = true;
		*args_lst = (*args_lst)->next;
	}
	return (is_valid_option);
}

/* print_arguments()
 *	
 *	- if argument is a tilde (~), replace by HOME directory
 *	- if valid '-n' option, print arguments close to prompt (no newline)
 *	- no valid '-n' option, print a space after every arg except the last one 
 *	- no options, newline after the last argument has been printed
 */
void	print_arguments(t_env_lst *env_lst, token *args_lst, \
						bool is_valid_option)
{
	while (args_lst != NULL)
	{
		if (args_lst->content[0] == '~' && !args_lst->content[1])
		{
			printf("%s\n", get_env_var_val(env_lst, "HOME"));
			is_valid_option = true;
		}
		else
		{
			printf("%s", args_lst->content);
			if (args_lst->next != NULL)
				printf(" ");
		}
		args_lst = args_lst->next;
	}
	if (!is_valid_option)
		printf("\n");
}

int	echo_builtin(t_env_lst *env_lst, token *args_lst)
{
	bool	is_valid_option;

	if (!args_lst)
		printf("\n");
	else
	{
		is_valid_option = process_options(&args_lst);
		print_arguments(env_lst, args_lst, is_valid_option);
	}
	return (EXIT_SUCCESS);
}
