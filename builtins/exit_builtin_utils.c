/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_builtin_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 00:03:48 by ccarrace          #+#    #+#             */
/*   Updated: 2023/11/30 00:15:39 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	is_valid_numeric(const char *str)
{
	if (*str == '\0')
		return (false);
	if (*str == '-' || *str == '+')
		str++;
	while (*str != '\0')
	{
		if (*str < '0' || *str > '9')
			return (false);
		str++;
	}
	return (true);
}

/* remove_leading_zeros()
 *
 *  - find the first non-zero character
 *	- skip following zeros, if exist
 *	- copy the remaining characters back to original string
 *	- null-terminate the modified string
*/
void	remove_leading_zeros(char *str)
{
	char	*non_zero_ptr;
	int		i;

	non_zero_ptr = str;
	while (*non_zero_ptr == '0' && *(non_zero_ptr + 1) != '\0')
		non_zero_ptr++;
	i = 0;
	while (*non_zero_ptr != '\0')
	{
		str[i] = *non_zero_ptr++;
		i++;
	}
	str[i] = '\0';
}

void	remove_trailing_spaces(char *str)
{
	int	len;

	len = 0;
	if (*str != ' ')
	{
		len = ft_strlen(str);
		while (len > 0 && str[len - 1] == ' ')
			len--;
		str[len] = '\0';
	}
}
