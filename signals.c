/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 21:49:58 by ccarrace          #+#    #+#             */
/*   Updated: 2023/12/13 23:01:07 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void		new_prompt_line(int signo);
static void		print_new_line(int signo);
static void		print_quit_msg(int signo);
void			disable_control_chars_echo(void);

/*	set_interactive_signals()
 *
 *	- replaces SIGINT  (CTL + C) default action by 'new_prompt_line()'
 *	- replaces SIGQUIT (CTL + \) default action by SIG_IGN (ignore signal)
 */
void	set_interactive_signals(void)
{
	struct sigaction	sigint_action;
	struct sigaction	sigquit_action;

	sigint_action.sa_handler = &new_prompt_line;
	sigemptyset(&sigint_action.sa_mask);
	sigint_action.sa_flags = 0;
	sigaction(SIGINT, &sigint_action, NULL);
	sigquit_action.sa_handler = SIG_IGN;
	sigemptyset(&sigquit_action.sa_mask);
	sigquit_action.sa_flags = 0;
	sigaction(SIGQUIT, &sigquit_action, NULL);
}

/*	set_noninteractive_signals()
 *
 *	- replaces SIGINT  (CTL + C) default action by 'print_new_line()'
 *	- replaces SIGQUIT (CTL + \) default action by 'print_quit_msg()'
 */
void	set_noninteractive_signals(void)
{
	struct sigaction	sigint_action;
	struct sigaction	sigquit_action;

	sigint_action.sa_handler = &print_new_line;
	sigemptyset(&sigquit_action.sa_mask);
	sigquit_action.sa_flags = 0;
	sigaction(SIGINT, &sigint_action, NULL);
	sigquit_action.sa_handler = &print_quit_msg;
	sigemptyset(&sigquit_action.sa_mask);
	sigquit_action.sa_flags = 0;
	sigaction(SIGQUIT, &sigquit_action, NULL);
}

void	new_prompt_line(int signo)
{
	(void)signo;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_exitstatus = 1;
}

void	print_new_line(int signo)
{
	printf("\n");
	rl_on_new_line();
	g_exitstatus = 128 + signo;
}

void	print_quit_msg(int signo)
{
	printf("Quit: 3\n");
	rl_on_new_line();
	g_exitstatus = 128 + signo;
}
