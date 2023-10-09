/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkhrabro <vkhrabro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 22:58:44 by vkhrabro          #+#    #+#             */
/*   Updated: 2023/10/09 22:47:32 by vkhrabro         ###   ########.fr       */
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
#include "libft/libft.h"    // Contains 'tcsetattr()' and 'tcgetattr()'

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

#endif 