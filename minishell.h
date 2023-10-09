/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 22:58:44 by vkhrabro          #+#    #+#             */
/*   Updated: 2023/10/09 20:29:05 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <termios.h>    // Contains 'tcsetattr()' and 'tcgetattr()'

typedef enum 
{
    TOKEN_COMMAND,
    TOKEN_ARGUMENT,
    TOKEN_PIPE,
    TOKEN_REDIRECT_IN,
    TOKEN_REDIRECT_OUT,
    // ... add any other token types you need
}             tokentype;

typedef struct token token;  // Forward declaration

struct token
{
    char       *content;
    tokentype   type;
    token      *next;
};

void        init_signals(void);
void        rl_replace_line (const char *text, int clear_undo);
void        disable_control_chars_echo();
void        restore_terminal_settings();

#endif 