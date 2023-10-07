/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkhrabro <vkhrabro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 19:50:25 by vkhrabro          #+#    #+#             */
/*   Updated: 2023/07/21 00:52:46 by vkhrabro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <stdio.h>

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		i;
	int		j;
	char	*s3;
	char	*s4;

	i = ft_strlen((char *)s1);
	j = ft_strlen((char *)s2);
	s3 = (char *)malloc(sizeof(char) * (i + j + 1));
	if (s3 && s1 && s2)
	{
		s4 = s3;
		while (*s1 != '\0')
			*s3++ = *s1++;
		while (*s2 != '\0')
			*s3++ = *s2++;
		*s3 = '\0';
	}
	else
		return (NULL);
	return (s4);
}

/*int main(void)
{
	char *s1 = "HHH";
	char *s2 = "BB";
	char *s3 = ft_strjoin(s1, s2);
	printf("%s\n", s3);
	return (0);
}*/
