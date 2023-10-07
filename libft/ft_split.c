/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkhrabro <vkhrabro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 22:19:16 by vkhrabro          #+#    #+#             */
/*   Updated: 2023/07/21 00:52:32 by vkhrabro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "libft.h"
#include <stdlib.h>

char	**ft_split(char const *s, char c);

int	ft_words(const char *s, char c)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (s[i])
	{
		if (s[i] == c)
			i++;
		else
		{
			j++;
			while (s[i] != '\0' && s[i] != c)
				i++;
		}
	}
	return (j);
}

char	*ft_wd(const char *str, char c)
{
	int		i;
	char	*wd;

	i = 0;
	while (*str && *str == c)
		str++;
	while (str[i] && str[i] != c)
		i++;
	wd = malloc(sizeof(char) * (i + 1));
	if (wd == NULL)
		return (NULL);
	i = 0;
	while (str[i] && str[i] != c)
	{
		wd[i] = str[i];
		i++;
	}
	wd[i] = '\0';
	return (wd);
}

void	free_words(int i, char **ptr)
{
	while (i > 0)
	{
		free(ptr[i - 1]);
		i--;
	}
	free(ptr);
}

char	**ft_loop(char **ptr_words, char const *s, char c, int words)
{
	int	i;

	i = 0;
	while (i < words)
	{
		while (*s && *s == c)
			s++;
		ptr_words[i] = ft_wd(s, c);
		if (ptr_words[i] == NULL)
		{
			free_words(i, ptr_words);
			return (NULL);
		}
		while (*s && *s != c)
			s++;
		i++;
	}
	ptr_words[i] = NULL;
	return (ptr_words);
}

char	**ft_split(char const *s, char c)
{
	int		words;
	char	**ptr_words;

	if (!s)
		return (NULL);
	words = ft_words(s, c);
	ptr_words = (char **)malloc(sizeof(char *) * (words + 1));
	if (ptr_words == NULL)
		return (NULL);
	return (ft_loop(ptr_words, s, c, words));
}

/*int main(void)
{
	char **s;
	int i;

	i = 0;
	s = ft_split("Hello world how are you", ' ');
	while (s[i])
	{
		printf("%s\n", s[i]);
		i++;
	}
	return (0);
}*/
