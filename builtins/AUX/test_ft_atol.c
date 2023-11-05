#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>

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

// static int	find_max_len(char *str1, char *str2)
// {
// 	int	len1;
// 	int	len2;
// 	int	max_len;

// 	len1 = strlen(str1);
// 	len2 = strlen(str2);
// 	if (len1 > len2)
// 		max_len = len1;
// 	else
// 		max_len = len2;
// 	return (max_len);
// }

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
int	main(int argc, char **argv)
{
	long	nbr;
	char	*str;
	if (argc == 2)
	{
		nbr = ft_atol(argv[1]);
		str = ft_ltoa(nbr);
		if (strncmp(argv[1], str, find_max_len(argv[1], str)) != 0)
			printf ("The number is not long type\n");
		else
			printf("the number is %ld\n", nbr);
	}
	else
		printf("Wrong args!\n");
	return (0);
}