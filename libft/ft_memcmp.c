/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkhrabro <vkhrabro@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/21 20:03:46 by vkhrabro          #+#    #+#             */
/*   Updated: 2023/02/17 17:02:31 by vkhrabro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *buf1, const void *buf2, size_t count)
{
	unsigned char	*a;
	unsigned char	*b;
	size_t			i;

	a = (unsigned char *)buf1;
	b = (unsigned char *)buf2;
	i = 0;
	while (i < count)
	{
		if (a[i] == b[i])
			i++;
		else if (a[i] < b[i])
			return (a[i] - b[i]);
		else
			return (a[i] - b[i]);
	}
	return (0);
}
