/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 21:11:32 by ccarrace          #+#    #+#             */
/*   Updated: 2023/12/15 00:52:26 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_token(token *t)
{
	printf("Token content: '%s', Token type: %s\n", t->content, \
		token_type_to_string(t->type));
}

void	print_tokens(const token *tokens)
{
	while (tokens != NULL)
	{
		printf("Token: '%s', Type: %s\n", tokens->content, \
				token_type_to_string(tokens->type));
		tokens = tokens->next;
	}
}

void	print_env_list(t_env_lst **env_lst)
{
	t_env_lst	*current;

	current = *env_lst;
	while (current != NULL)
	{
		printf("%s=%s\n", current->var_name, current->var_value);
		current = current->next;
	}
}
