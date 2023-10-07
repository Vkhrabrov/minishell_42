/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkhrabro <vkhrabro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 19:46:59 by vkhrabro          #+#    #+#             */
/*   Updated: 2023/07/10 23:50:40 by vkhrabro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(char *str, int c)
{
	int	length;

	length = ft_strlen(str);
	if ((char)c == '\0')
		return (&str[length]);
	while (length >= 0)
	{
		if (str[length] == (char)c)
			return (&str[length]);
		length--;
	}
	return (NULL);
}
