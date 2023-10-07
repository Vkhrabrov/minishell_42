/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkhrabro <vkhrabro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 21:49:51 by vkhrabro          #+#    #+#             */
/*   Updated: 2023/07/10 23:55:27 by vkhrabro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

char	*ft_string(char *str, int i, int sign, int n)
{
	while (n != 0)
	{
		str[--i] = (char)(n % 10) *sign + '0';
		n = n / 10;
	}
	if (sign < 0)
		str[--i] = '-';
	return (str);
}

int	ft_size(int n)
{
	int	i;

	i = 0;
	if (n == 0)
		i = 1;
	while (n != 0)
	{
		i++;
		n = n / 10;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	int		i;
	char	*str;
	int		sign;

	i = ft_size(n);
	sign = 1;
	if (n < 0)
		sign = -1;
	i = i + (n <= 0);
	str = (char *)malloc(sizeof(char) * (i + 1));
	if (!str)
		return (NULL);
	if (n == 0)
	{
		str[0] = '0';
		str[1] = '\0';
		return (str);
	}
	str[i] = '\0';
	str = ft_string(str, i, sign, n);
	return (str);
}

/*int main()
{
	printf("%s\n", ft_itoa(-567890));
	return(0);
}*/
