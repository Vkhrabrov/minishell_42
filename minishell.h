/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkhrabro <vkhrabro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 22:58:44 by vkhrabro          #+#    #+#             */
/*   Updated: 2023/10/21 21:53:39 by vkhrabro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <termios.h>
#include "libft/libft.h" 

typedef enum 
{
    T_COMMAND,
    T_ARG,
    T_PIPE,
    T_NONE,
    T_REDIR_IN,
    T_REDIR_OUT,
    T_HEREDOC,
    T_APP_REDIR,
    T_VAR_EXP,
    T_BACKGROUND_EXEC,
    T_EXPAND_TO_EXIT,
    T_HEREDOC_DELIM,
    T_ENV_VAR,
}             tokentype;

typedef struct token token;

struct token
{
    char       *content;
    tokentype   type;
    token      *next;
};

typedef struct command_node command_node;

struct command_node 
{
    token           *command;
    token           *args;
    token           *redirect_in;
    token           *redirect_out;
    token           *var_expansion;
    token           *env_variable;
    char            *here_doc_content;

    command_node    *next;
};

typedef struct tokenizer_state 
{
    int i;
    tokentype prev_type;
    int expect_command;
    int expect_filename_after_redir;
    token *tokens;
} tokenizer_state;

void            init_signals(void);
void            rl_replace_line (const char *text, int clear_undo);
void            disable_control_chars_echo();
void            restore_terminal_settings();
command_node*   parse_line(token *tokens);
void            print_command_node(command_node* head);
void            free_command_node(command_node* node); 
void            reset_command_node(command_node* cmd);
token           *creat_token(const char* content, tokentype type);
token           *get_last_token(token *head);
void            add_to_list(token **head, token *new_token);
char            *substring(char *input_string, int start, int end);
char            *lex_quoted_string(char *input_string, int *i, char end_char);
void            print_token(token *t);
token           *tokenization(char *input);
void            handle_commands_and_args(char *input, tokenizer_state *state);
void            handle_special_tokens(char c, char *input, tokenizer_state *state);
void            handle_redirection(char c, char *input, tokenizer_state *state);
void            handle_quoted_string(char c, char *input, tokenizer_state *state);
int             if_redir(char c);

#endif 