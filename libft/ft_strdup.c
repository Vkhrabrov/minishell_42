/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/29 20:01:41 by vkhrabro          #+#    #+#             */
/*   Updated: 2024/01/15 21:38:20 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strdup(const char *s1)
{
	char	*s2;
	size_t	i;

	i = ft_strlen(s1) + 1;
	s2 = (char *)malloc(i);
	if (s2 == NULL)
		return (s2);
	if (s1 != NULL)
		ft_strlcpy(s2, s1, i);
	return (s2);
}
