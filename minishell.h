/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 22:58:44 by vkhrabro          #+#    #+#             */
/*   Updated: 2023/10/12 20:08:17 by ccarrace         ###   ########.fr       */
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
#include <limits.h>     // PATH_SIZE
#include "libft/libft.h"    

typedef enum 
{
    TOKEN_COMMAND,
    TOKEN_ARGUMENT,
    TOKEN_PIPE,
    TOKEN_REDIRECT_IN,
    TOKEN_REDIRECT_OUT,
    TOKEN_HERE_DOC,
    TOKEN_APPEND_REDIRECTION,
    TOKEN_BACKGROUND_EXEC,
    TOKEN_EXPAND_TO_EXIT,
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
extern int  rl_eof_found;

#endif 