/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkhrabro <vkhrabro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 20:47:01 by vkhrabro          #+#    #+#             */
/*   Updated: 2023/07/21 00:51:59 by vkhrabro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

void	*ft_memset(void *ptr, int x, size_t n)
{
	unsigned char	*p;

	p = (unsigned char *)ptr;
	while (n--)
	{
		*p = (unsigned char)x;
		p++;
	}
	return (ptr);
}
