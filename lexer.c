/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkhrabro <vkhrabro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/21 21:51:03 by vkhrabro          #+#    #+#             */
/*   Updated: 2024/01/30 20:35:54 by vkhrabro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_redirection_end(char c, struct tokenizer_state *state)
{
	if (c == '>')
		state->prev_type = T_REDIR_OUT;
	else
		state->prev_type = T_REDIR_IN;
	state->expect_filename_after_redir = 1;
}

void	handle_redirection(char c, char *input, struct tokenizer_state *state)
{
	char	next_char;
	char	redir[3];

	next_char = input[state->i + 1];
	if ((c == '>' && next_char == '>') || (c == '<' && next_char == '<'))
	{
		redir[0] = c;
		redir[1] = next_char;
		redir[2] = '\0';
		if (c == '>')
			add_to_list(&(state->tokens), creat_token(redir, T_APP_REDIR));
		else
		{
			add_to_list(&(state->tokens), creat_token(redir, T_HEREDOC));
			state->prev_type = T_HEREDOC;
			state->i++;
			return ;
		}
		state->i++;
	}
	else
		handle_single_redirection(redir, c, state);
	handle_redirection_end(c, state);
}

void	handle_special_tokens(char c, char *input, \
		struct tokenizer_state *state)
{
	char	next_char;
	char	redir[3];

	next_char = input[state->i + 1];
	if (c == '|')
	{
		add_to_list(&(state->tokens), creat_token("|", T_PIPE));
		state->prev_type = T_PIPE;
		state->expect_command = 1;
	}
	else if (c == '$' && next_char == '?')
	{
		redir[0] = c;
		redir[1] = next_char;
		redir[2] = '\0';
		add_to_list(&(state->tokens), creat_token(redir, T_EX_ST));
		state->i++;
	}
	else if (c == '$')
	{
		add_to_list(&(state->tokens), creat_token("$", T_VAR_EXP));
		state->prev_type = T_VAR_EXP;
	}
	else if (if_redir(c))
		handle_redirection(c, input, state);
}

void	process_input(char *input, struct tokenizer_state *state, char c)
{
	while (state->i < (int)ft_strlen(input))
	{
		c = input[state->i];
		if (c == ' ' || c == '\t')
		{
			state->i++;
			continue ;
		}
		if (c == '"' || c == '\'')
			handle_q_str(c, &input, state);
		else if (c == '|' || c == '$' || if_redir(c))
		{
			if ((input[state->i + 1] == '"' || input[state->i + 1] == '\'')
				&& !if_redir(c))
			{
				state->i++;
				continue ;
			}
			else
				handle_special_tokens(c, input, state);
		}
		else
			handle_commands_and_args(input, state);
		state->i++;
	}
}

struct token	*tokenization(char *input)
{
	char					c;
	struct tokenizer_state	state;

	c = '\0';
	state = init_tokenizer_state();
	process_input(input, &state, c);
	return (state.tokens);
}
