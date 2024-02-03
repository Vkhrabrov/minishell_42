/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utility_functions_2.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 23:34:43 by vkhrabro          #+#    #+#             */
/*   Updated: 2024/01/31 15:57:35 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

enum tokentype	c_a_part_2(struct tokenizer_state *state, \
	enum tokentype current_type)
{
	if (state->expect_command && !state->expect_filename_after_redir)
	{
		current_type = T_CMD;
		state->expect_command = 0;
	}
	else
		current_type = T_ARG;
	if (state->expect_filename_after_redir)
	{
		current_type = T_ARG;
		state->expect_filename_after_redir = 0;
	}
	if (state->prev_type == T_VAR_EXP)
		current_type = T_ENV_VAR;
	if (state->prev_type == T_HEREDOC)
	{
		current_type = T_HEREDOC_DELIM;
		state->expect_command = 1;
	}
	return (current_type);
}

void	handle_commands_and_args(char *input, struct tokenizer_state *state)
{
	int				start;
	enum tokentype	current_type;
	char			*command_or_arg;
	struct token	*new_token;

	start = state->i;
	current_type = T_CMD;
	while (state->i < (int)ft_strlen(input) && !if_redir(input[state->i]) \
		&& input[state->i] != ' ' && input[state->i] != '|' \
		&& input[state->i] != '$')
		state->i++;
	command_or_arg = substring(input, start, state->i - 1);
	current_type = c_a_part_2(state, current_type);
	new_token = creat_token(command_or_arg, current_type);
	add_to_list(&(state->tokens), new_token);
	free(command_or_arg);
	state->prev_type = current_type;
	state->i--;
}

struct tokenizer_state	init_tokenizer_state(void)
{
	struct tokenizer_state	state;

	state.i = 0;
	state.prev_type = T_NONE;
	state.expect_command = 1;
	state.expect_filename_after_redir = 0;
	state.tokens = NULL;
	state.s_quote = 0;
	return (state);
}

void	add_to_list(struct token **head, struct token *new_token)
{
	struct token	*last_token;

	if (!*head)
		*head = new_token;
	else
	{
		last_token = get_last_token(*head);
		last_token->next = new_token;
		new_token->prev = last_token;
	}
}

char	*substring(char *input_string, int start, int end)
{
	char	*substring;
	int		k;
	int		t;

	k = end - start + 1;
	t = 0;
	substring = malloc(sizeof(char) * k + 1);
	while (t < k)
		substring[t++] = input_string[start++];
	substring[t] = '\0';
	return (substring);
}
