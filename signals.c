/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 21:49:58 by ccarrace          #+#    #+#             */
/*   Updated: 2024/01/31 00:22:48 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*	set_interactive_signals()
 *
 *	- replaces SIGINT  (CTL + C) default action by 'new_prompt_line()'
 *	- replaces SIGQUIT (CTL + \) default action by SIG_IGN (ignore signal)
 */
void	set_interactive_signals(void)
{
	struct sigaction	new_sigint_action;
	struct sigaction	new_sigquit_action;

	new_sigint_action.sa_handler = &new_prompt_line;
	sigemptyset(&new_sigint_action.sa_mask);
	new_sigint_action.sa_flags = 0;
	sigaction(SIGINT, &new_sigint_action, NULL);
	new_sigquit_action.sa_handler = SIG_IGN;
	sigemptyset(&new_sigquit_action.sa_mask);
	new_sigquit_action.sa_flags = 0;
	sigaction(SIGQUIT, &new_sigquit_action, NULL);
}

/*	set_noninteractive_signals()
 *
 *	- replaces SIGINT  (CTL + C) default action by 'print_new_line()'
 *	- replaces SIGQUIT (CTL + \) default action by 'print_quit_msg()'
 */
void	set_noninteractive_signals(void)
{
	struct sigaction	new_sigint_action;
	struct sigaction	new_sigquit_action;

	new_sigint_action.sa_handler = &print_new_line;
	sigemptyset(&new_sigint_action.sa_mask);
	new_sigint_action.sa_flags = 0;
	sigaction(SIGINT, &new_sigint_action, NULL);
	new_sigquit_action.sa_handler = &print_quit_msg;
	sigemptyset(&new_sigquit_action.sa_mask);
	new_sigquit_action.sa_flags = 0;
	sigaction(SIGQUIT, &new_sigquit_action, NULL);
}

void	set_heredoc_signals(void)
{
	struct sigaction	new_sigint_action;
	struct sigaction	new_sigquit_action;

	new_sigint_action.sa_handler = SIG_IGN;
	sigemptyset(&new_sigint_action.sa_mask);
	new_sigint_action.sa_flags = 0;
	sigaction(SIGINT, &new_sigint_action, NULL);
	new_sigquit_action.sa_handler = SIG_IGN;
	sigemptyset(&new_sigquit_action.sa_mask);
	new_sigquit_action.sa_flags = 0;
	sigaction(SIGQUIT, &new_sigquit_action, NULL);
}
