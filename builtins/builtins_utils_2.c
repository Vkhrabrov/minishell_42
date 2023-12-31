/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/26 14:38:44 by ccarrace          #+#    #+#             */
/*   Updated: 2023/12/20 14:07:25 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_args_list(struct token *args_lst)
{
	struct token	*current;

	while (args_lst != NULL)
	{
		current = args_lst;
		free(current->content);
		free(current);
		args_lst = args_lst->next;
	}
}

long	ft_atol(const char *str)
{
	long	result;
	int		sign;

	result = 0;
	sign = 1;
	while (*str == 32 || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		result = (result * 10) + (*str - '0');
		str++;
	}
	return (result * sign);
}

/*	Start of ft_ltoa (long to char)	and complementary functions */
static long	ft_long_length(long n)
{
	long	len;

	if (n <= 0)
		len = 1;
	else
		len = 0;
	while (n != 0)
	{
		n = n / 10;
		len++;
	}
	return (len);
}

static char	*ft_long_min(void)
{
	char	*str;
	char	*tmp;
	int		i;

	i = 0;
	str = malloc(sizeof(char) * 21);
	if (!str)
		return (NULL);
	tmp = "-9223372036854775808";
	while (i < 20)
	{
		str[i] = tmp[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

// (99) LL suffix ensures number is interpreted as a long integer literal
char	*ft_ltoa(long n)
{
	long	len;
	char	*str;

	len = ft_long_length(n);
	if (n == -9223372036854775807LL - 1)
		return (ft_long_min());
	str = malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	str[len] = '\0';
	if (n == 0)
		str[0] = n + '0';
	if (n < 0)
	{
		str[0] = '-';
		n = n * (-1);
	}
	while (n > 0)
	{
		str[len - 1] = (n % 10) + '0';
		n = n / 10;
		len--;
	}
	return (str);
}
/*	End of ft_ltoa */
