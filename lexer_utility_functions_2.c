/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utility_functions_2.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkhrabro <vkhrabro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 23:34:43 by vkhrabro          #+#    #+#             */
/*   Updated: 2023/12/17 22:22:01 by vkhrabro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void add_to_list(token **head, token *new_token) 
{
    token *last_token;

    if (!*head) {
        *head = new_token;
    } else {
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

tokentype	c_a_part_2(tokenizer_state *state, tokentype current_type)
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

void	handle_commands_and_args(char *input, tokenizer_state *state)
{
	int			start;
	tokentype	current_type;
	char		*command_or_arg;

	start = state->i;
	current_type = T_CMD;
	while (state->i < (int)ft_strlen(input) && !if_redir(input[state->i])
		&& input[state->i] != ' ' && input[state->i] != '|')
		state->i++;
	command_or_arg = substring(input, start, state->i - 1);
	current_type = c_a_part_2(state, current_type);
	add_to_list(&(state->tokens), creat_token(command_or_arg, current_type));
	free(command_or_arg);
	state->prev_type = current_type;
	state->i--;
}

tokenizer_state	init_tokenizer_state(void)
{
	tokenizer_state	state;

	state.i = 0;
	state.prev_type = T_NONE;
	state.expect_command = 1;
	state.expect_filename_after_redir = 0;
	state.tokens = NULL;
	return (state);
}
