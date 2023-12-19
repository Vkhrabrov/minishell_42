/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 21:15:47 by ccarrace          #+#    #+#             */
/*   Updated: 2023/12/18 18:41:41 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	if_redir(char c)
{
	if (c == '>' || c == '<')
		return (1);
	else
		return (0);
}
int	is_prev_pipe_or_none(tokenizer_state *state, char *arg)
{
	return ((state->prev_type == T_PIPE || state->prev_type == T_NONE)
		&& ft_strlen(arg) != 0);
}

int	is_prev_redir(tokenizer_state *state, char *arg)
{
	return ((state->prev_type == T_REDIR_IN || state->prev_type == T_REDIR_OUT)
		&& ft_strlen(arg) != 0);
}

void	handle_quoted_string(char c, char *input, tokenizer_state *state)
{
	char		end_char;
	char		*arg;
	tokentype	current_type;

	end_char = c;
	arg = lex_quoted_string(input, &(state->i), end_char);
	current_type = T_ARG;
	if (ft_strlen(arg) == 0 && state->prev_type == T_CMD)
		current_type = T_ARG;
	else if (ft_strlen(arg) == 0 && ft_strlen(input) == 2)
		current_type = T_CMD;
	else if (ft_strlen(arg) == 0 && ft_strlen(input) > 2)
		return ;
	if (is_prev_pipe_or_none(state, arg))
	{
		current_type = T_CMD;
		state->expect_command = 0;
	}
	else if (is_prev_redir(state, arg))
		current_type = T_ARG;
	else if (state->prev_type == T_VAR_EXP && ft_strlen(arg) != 0)
		current_type = T_ENV_VAR;
	add_to_list(&(state->tokens), creat_token(arg, current_type));
	free(arg);
	state->prev_type = current_type;
}
