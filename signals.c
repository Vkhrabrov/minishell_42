/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 21:49:58 by ccarrace          #+#    #+#             */
/*   Updated: 2023/10/07 00:24:04 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void new_prompt_line()
{   
    printf("\n");
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

void init_signals(void)
{
    struct sigaction    sigint_action;  // Replaces default 'ctrl + c'
    struct sigaction    sigquit_action; // Replaces default 'ctrl + \'
    struct sigaction    eof_action;     // Replaces default 'ctrl + d'

    // Setup the SIGINT handler
    sigint_action.sa_handler = new_prompt_line;
    sigemptyset(&sigint_action.sa_mask);
    sigint_action.sa_flags = 0;
    sigaction(SIGINT, &sigint_action, NULL);
    // Setup the SIGQUIT handler
    sigint_action.sa_handler = new_prompt_line;
    sigemptyset(&sigint_action.sa_mask);
    sigint_action.sa_flags = 0;
    sigaction(SIGQUIT, &sigquit_action, NULL);
    // Setup the EOF (ctrl + 'd') handler
    eof_action.sa_handler = new_prompt_line;
    sigemptyset(&sigint_action.sa_mask);
    sigint_action.sa_flags = 0;
    sigaction(SIGINT, &sigint_action, NULL);
} 
