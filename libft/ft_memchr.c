/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkhrabro <vkhrabro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 17:50:45 by vkhrabro          #+#    #+#             */
/*   Updated: 2023/07/21 00:51:49 by vkhrabro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

void	*ft_memchr(const void *str, int c, size_t n)
{
	size_t			i;
	unsigned char	*b;
	unsigned char	obj;

	i = 0;
	b = (unsigned char *)str;
	obj = (unsigned char)c;
	while (i < n)
	{
		if (*b == obj)
			return (b);
		b++;
		i++;
	}
	return (NULL);
}
