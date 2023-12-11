/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 21:49:58 by ccarrace          #+#    #+#             */
/*   Updated: 2023/12/11 23:05:08 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
    The static variable 'original_termios' is used to preserve and restore 
    terminal settings, which is a separate concern from signal handling. Its use
    is not violating the subject rules, since it is not related to indicating a 
    received signal at all.
*/
static struct termios original_termios;
static void new_prompt_line(int signo);
static void	print_new_line(int signo);
static void	print_quit_msg(int signo);
void disable_control_chars_echo(void);

void	set_interactive_signals(void)
{
    struct sigaction    sigint_action;  // Replaces default 'ctrl + c'
    struct sigaction    sigquit_action; // Replaces default 'ctrl + \'

    // Setup the SIGINT ('ctrl + c') handler
    sigint_action.sa_handler = &new_prompt_line;
    sigemptyset(&sigint_action.sa_mask);
    sigint_action.sa_flags = 0;
    sigaction(SIGINT, &sigint_action, NULL);

    // Setup the SIGQUIT ('ctrl + \') handler to ignore the signal
    sigquit_action.sa_handler = SIG_IGN;
    sigemptyset(&sigquit_action.sa_mask);
    sigquit_action.sa_flags = 0;
    sigaction(SIGQUIT, &sigquit_action, NULL);
}

void	set_noninteractive_signals(void)
{
	struct sigaction	sigint_action;
	struct sigaction	sigquit_action;
	
    // Setup the SIGINT ('ctrl + c') handler to quit
	sigint_action.sa_handler = &print_new_line;
	sigemptyset(&sigquit_action.sa_mask);
	sigquit_action.sa_flags = 0;
	sigaction(SIGINT, &sigint_action, NULL);
	
    // Setup the SIGQUIT ('ctrl + \') handler to quit
    sigquit_action.sa_handler = &print_quit_msg;
    sigemptyset(&sigquit_action.sa_mask);
    sigquit_action.sa_flags = 0;
    sigaction(SIGQUIT, &sigquit_action, NULL);
}

void new_prompt_line(int signo)
{
    printf("\n");
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
	g_exitstatus = 128 + signo;
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
/*
    disable_control_chars_echo()
    
    The extra characters (^C or ^), often referred to as control characters,
    are added to the terminal's input buffer when you press CTRL+C or CTRL+\ 
    to send SIGINT or SIGQUIT signals. To prevent these control characters 
    from being displayed in your minishell's prompt, you can configure your 
    terminal to not echo control characters.

    You can achieve this by setting the terminal's attributes, specifically 
    the ECHOCTL flag, using the termios library in C. 
*/
void disable_control_chars_echo(void) 
{
    struct termios new_termios; 
    tcgetattr(0, &original_termios);
    new_termios = original_termios;
    new_termios.c_lflag &= ~ECHOCTL; // Disable echoing of control characters (^C, ^\)
    tcsetattr(0, TCSANOW, &new_termios);
}

void restore_terminal_settings(void) 
{
    tcsetattr(0, TCSANOW, &original_termios);
}
