/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 22:31:52 by ccarrace          #+#    #+#             */
/*   Updated: 2023/12/18 19:41:40 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
    The static variable 'original_termios' is used to preserve and restore 
    terminal settings, which is a separate concern from signal handling. Its use
    is not violating the subject rules, since it is not related to indicating a 
    received signal at all.
*/
// static struct termios	g_original_termios;

/*	disable_control_chars_echo()
 *   
 *  The extra characters ^C and ^\ (a.k.a control characters) are added to the
 *	terminal's input buffer when you press CTRL+C or CTRL+\ to send SIGINT or 
 *	SIGQUIT signals. To prevent these control characters from being displayed 
 *	in minishell's prompt we must configure the terminal to not echo control
 *	characters. This can achieve this by setting the terminal's attributes, 
 *	specifically the ECHOCTL flag, using the termios library in C. 
 */
void	disable_control_chars_echo(void)
{
	struct termios	new_termios;

	tcgetattr(0, &new_termios);
	new_termios.c_lflag &= ~ECHOCTL;
	tcsetattr(0, TCSANOW, &new_termios);
}

/*	restore_terminal_settings()
 *
 *	Sets terminal configuration back to the original settings
 */
void	restore_terminal_settings(void)
{
	struct termios	new_termios;
	
	tcgetattr(0, &new_termios);
	new_termios.c_lflag |= ECHOCTL;
	tcsetattr(0, TCSANOW, &new_termios);
}
