/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkhrabro <vkhrabro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/21 21:51:03 by vkhrabro          #+#    #+#             */
/*   Updated: 2023/10/21 21:53:30 by vkhrabro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void handle_quoted_string(char c, char *input, tokenizer_state *state) 
{
    char end_char = c;
    char *arg = lex_quoted_string(input, &(state->i), end_char);

    tokentype current_type = T_ARG;
    if (state->prev_type == T_PIPE || state->prev_type == T_NONE
        || state->prev_type == T_REDIR_IN
        || state->prev_type == T_REDIR_OUT)
        current_type = T_COMMAND;
    else if (state->prev_type == T_VAR_EXP)
        current_type = T_ENV_VAR;

    add_to_list(&(state->tokens), creat_token(arg, current_type));
    free(arg);

    state->prev_type = current_type;
}

void handle_redirection(char c, char *input, tokenizer_state *state)
{
    char next_char = input[state->i + 1];
    if ((c == '>' && next_char == '>') || (c == '<' && next_char == '<')) 
    {
        char redir[3] = {c, next_char, '\0'};
        if (c == '>')
            add_to_list(&(state->tokens), creat_token(redir, T_APP_REDIR));
        else
            add_to_list(&(state->tokens), creat_token(redir, T_HEREDOC));
        state->i++;
    } 
    else 
    {
        char redir[2] = {c, '\0'};
        if (c == '>')
            add_to_list(&(state->tokens), creat_token(redir, T_REDIR_IN));
        else
            add_to_list(&(state->tokens), creat_token(redir, T_REDIR_OUT));
    }
        if (c == '>')
            state->prev_type = T_REDIR_OUT;
        else
            state->prev_type = T_REDIR_IN;
        state->expect_filename_after_redir = 1;
}

void handle_special_tokens(char c, char *input, tokenizer_state *state)
{
     if (c == '|') 
    {
        add_to_list(&(state->tokens), creat_token("|", T_PIPE));
        state->prev_type = T_PIPE;
        state->expect_command = 1;
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

void handle_commands_and_args(char *input, tokenizer_state *state)
{
    int start = state->i;
    tokentype current_type = T_COMMAND;
    while (state->i < (int)ft_strlen(input) && !if_redir(input[state->i])
        && input[state->i] != ' ' && input[state->i] != '|') 
        state->i++;
    char *command_or_arg = substring(input, start, state->i - 1);
        if (state->expect_command) 
        {
            current_type = T_COMMAND;
            state->expect_command = 0;
        } 
        else 
            current_type = T_ARG;
        if (state->expect_filename_after_redir) 
        {
            current_type = T_ARG;
            state->expect_filename_after_redir = 0;
            state->expect_command = 1;
        }
        if (state->prev_type == T_VAR_EXP)
            current_type = T_ENV_VAR;
        add_to_list(&(state->tokens),
            creat_token(command_or_arg, current_type));
        free(command_or_arg);
        state->prev_type = current_type;
        state->i--;
}

token *tokenization(char *input) 
{
    tokenizer_state state = {0, T_NONE, 1, 0, NULL};

    while (state.i < (int)ft_strlen(input)) 
    {
        char c = input[state.i];
        if (c == ' ' || c == '\t') 
        {
            state.i++;
            continue;
        }
        if (c == '"' || c == '\'') 
            handle_quoted_string(c, input, &state);
        else if (c == '|' || c == '$' || if_redir(c))
            handle_special_tokens(c, input, &state);
        else 
            handle_commands_and_args(input, &state);
        state.i++;
    }
    return state.tokens;
}