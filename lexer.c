/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkhrabro <vkhrabro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/21 21:51:03 by vkhrabro          #+#    #+#             */
/*   Updated: 2023/12/11 23:22:48 by vkhrabro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_quoted_string(char c, char *input, tokenizer_state *state)
{
	char		end_char;
	char		*arg;
	tokentype	current_type;

	end_char = c;
	arg = lex_quoted_string(input, &(state->i), end_char);
	current_type = T_ARG;
	if (ft_strlen(arg) == 0 && state->prev_type == T_CMD)
	{
		current_type = T_ARG;
	}
	else if (ft_strlen(arg) == 0 && ft_strlen(input) == 2)
	{
		current_type = T_CMD;
	}
	else if (ft_strlen(arg) == 0 && ft_strlen(input) > 2)
		return ;
	
	if ((state->prev_type == T_PIPE || state->prev_type == T_NONE) && ft_strlen(arg) != 0)
	{
		current_type = T_CMD;
		state->expect_command = 0;
	}
	else if ((state->prev_type == T_REDIR_IN || state->prev_type == T_REDIR_OUT) && ft_strlen(arg) != 0)

		current_type = T_ARG;
	else if (state->prev_type == T_VAR_EXP && ft_strlen(arg) != 0)
		current_type = T_ENV_VAR;
	add_to_list(&(state->tokens), creat_token(arg, current_type));
	free(arg);
	state->prev_type = current_type;
}

void	handle_redirection_end(char c, tokenizer_state *state)
{
	if (c == '>')
		state->prev_type = T_REDIR_OUT;
	else
		state->prev_type = T_REDIR_IN;
	state->expect_filename_after_redir = 1;
}

void handle_redirection(char c, char *input, tokenizer_state *state)
{
    char next_char;
    char redir[3];

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
            state->prev_type = T_HEREDOC;  // Set the state here for HEREDOC
            state->i++;  // Increment the index
            return;  // Exit the function, so we don't reset the state in handle_redirection_end
        }
        state->i++;
    }
    else
    {
        ft_strlcpy(redir, (char []){c, '\0', '\0'}, sizeof(redir));
        if (c == '>')
            add_to_list(&(state->tokens), creat_token(redir, T_REDIR_OUT));
        else
            add_to_list(&(state->tokens), creat_token(redir, T_REDIR_IN));
    }
    handle_redirection_end(c, state);
}


void	handle_special_tokens(char c, char *input, tokenizer_state *state)
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
		add_to_list(&(state->tokens), creat_token(redir, T_EXIT_STATUS));
		state->i++;
	}
	else if (c == '$')
	{
		add_to_list(&(state->tokens),
			creat_token("$", T_VAR_EXP));
		state->prev_type = T_VAR_EXP;
	}
	else if (if_redir(c))
		handle_redirection(c, input, state);
}
void print_tokens(const token *tokens) {
    while (tokens != NULL) {
        printf("Token: '%s', Type: %s\n", tokens->content, token_type_to_string(tokens->type));
        tokens = tokens->next;
    }
}

token	*tokenization(char *input)
{
	tokenizer_state	state;
	char			c;

	state = init_tokenizer_state();
	while (state.i < (int)ft_strlen(input))
	{
		c = input[state.i];
		if (c == ' ' || c == '\t')
		{
			state.i++;
			continue ;
		}
		if (c == '"' || c == '\'')
			handle_quoted_string(c, input, &state);
		else if (c == '|' || c == '$' || if_redir(c))
			handle_special_tokens(c, input, &state);
		else
			handle_commands_and_args(input, &state);
		state.i++;
	}
	// print_""tokens(state.tokens);
	return (state.tokens);
}
