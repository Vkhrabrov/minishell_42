/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 22:11:13 by ccarrace          #+#    #+#             */
/*   Updated: 2024/01/31 00:26:18 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	process_single_n_option(struct token **args_lst, bool *option_is_valid)
{
	if (*args_lst != NULL && ft_strncmp((*args_lst)->content, "-n", 2) == 0 \
			&& !(*args_lst)->content[2])
	{
		*option_is_valid = true;
		while (*args_lst != NULL && \
			ft_strncmp((*args_lst)->content, "-n", 2) == 0 && \
			!(*args_lst)->content[2])
			*args_lst = (*args_lst)->next;
	}
	return (*option_is_valid);
}

bool	process_multi_n_option(struct token **args_lst, bool *option_is_valid)
{
	int	i;

	while (*args_lst != NULL && ft_strncmp((*args_lst)->content, "-n", 2) == 0)
	{
		i = 1;
		while ((*args_lst)->content[i])
		{
			if ((*args_lst)->content[i] == 'n')
			{
				*option_is_valid = true;
				i++;
			}
			else
			{
				*option_is_valid = false;
				break ;
			}
		}
		if (*option_is_valid == true)
			*args_lst = (*args_lst)->next;
		else
			break ;
	}
	return (*option_is_valid);
}

/* process_options()
 *
 *	Checks if option '-n' passed is valid or not
 */
bool	process_options(struct token **args_lst)
{
	bool	option_is_valid;

	option_is_valid = false;
	if (*args_lst != NULL && ft_strncmp((*args_lst)->content, "-n", 2) == 0 \
			&& !(*args_lst)->content[2])
		option_is_valid = process_single_n_option(args_lst, &option_is_valid);
	else
		option_is_valid = process_multi_n_option(args_lst, &option_is_valid);
	return (option_is_valid);
}

/* print_arguments()
 *	
 *	- if argument is a tilde (~), replace by HOME directory
 *	- if valid '-n' option, print arguments close to prompt (no newline)
 *	- no valid '-n' option, print a space after every arg except the last one 
 *	- no options, newline after the last argument has been printed
 */
void	print_arguments(t_env_lst *env_lst, struct token *args_lst, \
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

int	echo_builtin(t_env_lst *env_lst, struct command_node *head)
{
	bool	is_valid_option;

	if (!head->args)
		printf("\n");
	else
	{
		is_valid_option = process_options(&head->args);
		print_arguments(env_lst, head->args, is_valid_option);
	}
	return (EXIT_SUCCESS);
}
