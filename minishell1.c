/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkhrabro <vkhrabro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 21:59:57 by ccarrace          #+#    #+#             */
/*   Updated: 2023/12/20 23:17:56 by vkhrabro         ###   ########.fr       */
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
