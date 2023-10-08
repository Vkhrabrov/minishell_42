/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vadimhrabrov <vadimhrabrov@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 22:58:44 by vkhrabro          #+#    #+#             */
/*   Updated: 2023/10/08 20:51:37 by vadimhrabro      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include "libft/libft.h"

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

#endif
