/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkhrabro <vkhrabro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 22:58:44 by vkhrabro          #+#    #+#             */
/*   Updated: 2023/10/20 00:10:38 by vkhrabro         ###   ########.fr       */
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
    TOKEN_COMMAND,
    TOKEN_ARGUMENT,
    TOKEN_PIPE,
    TOKEN_NONE,
    TOKEN_REDIRECT_IN,
    TOKEN_REDIRECT_OUT,
    TOKEN_HERE_DOC,
    TOKEN_APPEND_REDIRECTION,
    TOKEN_VARIABLE_EXPANSION,
    TOKEN_BACKGROUND_EXEC,
    TOKEN_EXPAND_TO_EXIT,
    TOKEN_HEREDOC_DELIM,
    TOKEN_ENV_VARIABLE,
}             tokentype;

typedef struct token token;

struct token
{
    char       *content;
    tokentype   type;
    token      *next;
};

typedef struct command_node command_node;

struct command_node {
    token           *command;
    token           *args;
    token           *redirect_in;
    token           *redirect_out;
    token           *var_expansion;
    token           *env_variable;
    char            *here_doc_content;

    command_node    *next;
};

void            init_signals(void);
void            rl_replace_line (const char *text, int clear_undo);
void            disable_control_chars_echo();
void            restore_terminal_settings();
command_node*   parse_line(token *tokens);
void            print_command_node(command_node* head);
void            free_command_node(command_node* node); 
void            reset_command_node(command_node* cmd);

#endif 