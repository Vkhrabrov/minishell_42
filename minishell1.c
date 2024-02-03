/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 21:59:57 by ccarrace          #+#    #+#             */
/*   Updated: 2024/02/01 00:03:42 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	arg_alone(struct command_node *head, int num_tokens)
{
	if (num_tokens == 1 \
		|| (head && head->command && head->command->content \
		&& ((ft_strncmp(head->command->content, "cat", 3) == 0 \
		&& ft_strlen(head->command->content) == 3) \
		|| (ft_strncmp(head->command->content, "wc", 2) == 0 \
		&& ft_strlen(head->command->content) == 2))))
		return (1);
	return (0);
}

void	bash_exit_emulate(t_env_lst *env_lst)
{
	free_env_list(env_lst);
	ft_putstr_fd("\033[A", 1);
	ft_putstr_fd("\0\33[2K", 1);
	printf("%s", "minishell> exit\n");
}
