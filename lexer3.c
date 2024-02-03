/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkhrabro <vkhrabro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 22:39:24 by vkhrabro          #+#    #+#             */
/*   Updated: 2024/01/25 23:05:46 by vkhrabro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	if_redir(char c)
{
	if (c == '>' || c == '<')
		return (1);
	else
		return (0);
}

void	handle_single_redirection(char *redir, char c, \
		struct tokenizer_state *state)
{
	ft_strlcpy(redir, (char []){c, '\0', '\0'}, sizeof(redir));
	if (c == '>')
		add_to_list(&(state->tokens), creat_token(redir, T_REDIR_OUT));
	else
		add_to_list(&(state->tokens), creat_token(redir, T_REDIR_IN));
}
