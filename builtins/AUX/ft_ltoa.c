#include <stdlib.h>
#include <stdio.h>

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
	// LL suffix ensures number is interpreted as a long integer literal.
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
