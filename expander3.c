/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkhrabro <vkhrabro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 00:00:12 by vkhrabro          #+#    #+#             */
/*   Updated: 2024/02/01 20:54:08 by vkhrabro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

struct token	*create_new_token(char *content, enum tokentype type)
{
	struct token	*new_token;

	new_token = (struct token *)malloc(sizeof(struct token));
	if (!new_token)
		return (NULL);
	new_token->content = ft_strdup(content);
	if (!new_token->content)
	{
		free(new_token);
		return (NULL);
	}
	new_token->type = type;
	new_token->next = NULL;
	return (new_token);
}

struct token	*create_new_tokens(struct token **prev,
	struct token *current, char *env_value, struct command_node *command)
{
	struct token	*new_token;

	new_token = create_new_token(env_value, T_ARG);
	new_token->next = current->next;
	if (*prev)
		(*prev)->next = new_token;
	else
		command->command = new_token;
	if (current->next)
		current->next->prev = new_token;
	new_token->prev = *prev;
	return (new_token);
}

void	rpl_env_with_tkn(struct command_node *current_command,
		char *env_value)
{
	insert_new_token(current_command, env_value);
	cleanup_old_data(current_command);
}

void	advance_token_pointers(struct token **prev, struct token **current)
{
	*prev = *current;
	*current = (*current)->next;
}

void	token_insert(struct command_node *command, struct token *new_token)
{
	if (!new_token)
		return ;
	if (command->args == NULL)
		command->args = new_token;
}
