#include <stdio.h>
#include <string.h>

char *flank_string(char *str, char init_c, char end_c)
{
	int	i;
	int	len;

	len = strlen(str);
	i = len;
	while (i >= 0)
	{
		str[i + 1] = str[i];
		i--;
	}
	// str[0] = '`';
	len = strlen(str);
	// str[len] = '"';
	str[len + 1] = '\0';
	return (str);
}

int main(int argc, char **argv)
{
	(void)argc;

    printf("Original string: %s\n", argv[1]);
    char *result = flank_string(argv[1], *argv[2], *argv[3]);
    printf("Modified string: %s\n", result);

    return 0;
}
