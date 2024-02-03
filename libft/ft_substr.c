/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkhrabro <vkhrabro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/29 20:37:49 by vkhrabro          #+#    #+#             */
/*   Updated: 2023/07/21 00:53:10 by vkhrabro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char			*s2;
	unsigned int	i;

	i = ft_strlen(s);
	if (start > i)
	{
		s2 = malloc(sizeof(char) * 1);
		if (s2 == NULL)
			return (0);
		*s2 = '\0';
		return (s2);
	}
	if (len > i - start)
		len = i - start;
	s2 = malloc(sizeof(char) * (len + 1));
	if (s2 == NULL)
		return (0);
	if (s != NULL)
		ft_strlcpy(s2, &s[start], len + 1);
	return (s2);
}
