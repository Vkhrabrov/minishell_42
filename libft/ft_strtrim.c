/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkhrabro <vkhrabro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 18:06:06 by vkhrabro          #+#    #+#             */
/*   Updated: 2023/07/10 23:57:23 by vkhrabro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*s2;
	int		i;
	int		j;

	i = ft_strlen(s1);
	j = 0;
	if (*s1 == '\0')
		return (ft_strdup(""));
	if (s1 != 0 && set != 0)
	{
		while (s1[j] && ft_strchr(set, s1[j]))
			j++;
		while (s1[i - 1] && ft_strchr(set, s1[i - 1]) && i > j)
			i--;
		s2 = ((char *)malloc(sizeof(char) * (i - j + 1)));
		if (s2)
		{
			ft_strlcpy(s2, &s1[j], i - j + 1);
			return (s2);
		}
	}
	return (NULL);
}
