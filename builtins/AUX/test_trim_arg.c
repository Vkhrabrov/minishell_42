#include <string.h>
#include <stdio.h>
#include <ctype.h>

char	*trim_first_arg(char *str)
{
	char	*trimmed_str;
	int		len;

	trimmed_str = str;
	if (*trimmed_str == '"' || *trimmed_str == '\'')
		trimmed_str++;
	len = strlen(trimmed_str);
	while (len > 0 && *trimmed_str == ' ')
	{
		trimmed_str++;
		len--;
	}
	trimmed_str[len] = '\0';
	if (!isdigit(*trimmed_str))
		return (str);
	else
		return (trimmed_str);
}

int	main(int argc, char **argv)
{
	char	*trimmed_arg;

	if (argc == 2)
	{
		trimmed_arg = trim_first_arg(argv[1]);
		printf("The trimmed argument is %s\n", trimmed_arg);
	}
	return (0);
}