/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkhrabro <vkhrabro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 21:15:47 by ccarrace          #+#    #+#             */
/*   Updated: 2024/02/01 21:37:55 by vkhrabro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_prev_pipe_or_none(struct tokenizer_state *state, char *arg)
{
	return ((state->prev_type == T_PIPE || state->prev_type == T_NONE)
		&& ft_strlen(arg) != 0);
}

int	is_prev_redir(struct tokenizer_state *state, char *arg)
{
	return ((state->prev_type == T_REDIR_IN || state->prev_type == T_REDIR_OUT)
		&& ft_strlen(arg) != 0);
}

enum tokentype	determine_current_type(char *arg,
	struct tokenizer_state *state, char *input)
{
	enum tokentype	current_type;

	current_type = T_ARG;
	if (ft_strlen(arg) == 0)
	{
		if (state->prev_type == T_CMD)
			current_type = T_ARG;
		else if (ft_strlen(input) == 2)
			current_type = T_CMD;
		else if (ft_strlen(input) > 2)
			return ((enum tokentype) - 1);
	}
	if (is_prev_pipe_or_none(state, arg))
	{
		current_type = T_CMD;
		state->expect_command = 0;
	}
	else if (is_prev_redir(state, arg))
		current_type = T_ARG;
	else if (state->prev_type == T_VAR_EXP && ft_strlen(arg) != 0)
		current_type = T_ENV_VAR;
	return (current_type);
}

int	new_function(char c, char **input, struct tokenizer_state *state, char *arg)
{
	if (c == '"' && ft_strchr(arg, '$') != NULL && ft_strchr(arg, '|') == NULL)
	{
		state->i = 0;
		process_input(arg, state, c);
		state->i = ft_strlen(*input);
		free(arg);
		return (1);
	}
	if (c == '\'' && (ft_strchr(arg, '$') != NULL))
	{
		state->current_type = T_ARG;
		add_to_list(&(state->tokens), creat_token(arg, state->current_type));
		free(arg);
		state->prev_type = state->current_type;
		return (1);
	}
	if (ft_strchr(arg, '$') != NULL && ft_strlen(arg) < 6 && ft_strlen(arg) > 3)
	{
		if (*input)
			ft_strlcpy(*input, arg, ft_strlen(arg));
		free(arg);
		state->i = -1;
		return (1);
	}
	return (0);
}

void	handle_q_str(char c, char **input, struct tokenizer_state *state)
{
	char			end_char;
	char			*arg;

	end_char = c;
	arg = lex_quoted_string(*input, &(state->i), end_char);
	if (!arg)
	{
		free(input);
		exit(1);
	}
	if (new_function(c, input, state, arg) == 1)
		return ;
	state->current_type = determine_current_type(arg, state, *input);
	if (state->current_type == (enum tokentype) - 1)
	{
		free(arg);
		return ;
	}
	add_to_list(&(state->tokens), creat_token(arg, state->current_type));
	free(arg);
	state->prev_type = state->current_type;
}
