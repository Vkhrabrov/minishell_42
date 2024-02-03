/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkhrabro <vkhrabro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/15 16:58:22 by vkhrabro          #+#    #+#             */
/*   Updated: 2023/07/21 00:51:54 by vkhrabro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char	*p;
	unsigned char	*b;

	p = (unsigned char *)dest;
	b = (unsigned char *)src;
	if (!dest && !src)
		return (NULL);
	while (n--)
	{
		*p = *b;
		p++;
		b++;
	}
	return (dest);
}
