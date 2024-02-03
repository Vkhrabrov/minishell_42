/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkhrabro <vkhrabro@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 18:40:24 by vkhrabro          #+#    #+#             */
/*   Updated: 2023/02/14 20:25:30 by vkhrabro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t	size)
{
	size_t	i;
	size_t	n;
	size_t	j;

	i = 0;
	n = 0;
	while (dest[i])
		i++;
	j = 0;
	while (src[j])
		j++;
	if (i >= size)
		return (size + j);
	while (src[n] && (i + n + 1) < size)
	{
		dest[i + n] = src[n];
		n++;
	}
	dest[i + n] = '\0';
	return (i + j);
}
