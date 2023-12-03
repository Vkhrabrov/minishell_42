#include <stdio.h>
#include <stdbool.h>

int	ft_isalpha(int c)
{
	if ((c > 64 && c < 91) || (c > 96 && c < 123))
		return (1);
	else
		return (0);
}

int	ft_isalnum(int c)
{
	if ((c > 64 && c < 91) || (c > 96 && c < 123) || (c > 47 && c < 58))
		return (1);
	else
		return (0);
}

size_t find_char_index(const char *str, char target) 
{
    size_t index;
    
    index = 0;
    while (str[index] != '\0')
	{
		if (str[index] == target)
			return (index);
        index++;
	}
	return (0);
}

static bool	is_var_name_valid(char *arg, int equal_sign_position)
{
	int	i;

	i = 0;
	if (!ft_isalpha(arg[i]) && arg[i] != '_')
		return (false);
	i++;
	while (i < equal_sign_position)
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

int	main(int argc, char **argv)
{
	(void)argc;
	
	if (is_var_name_valid(argv[1], find_char_index(argv[1], '=')) == true)
		printf("Name is valid\n");
	else
		printf("Invalid name\n");
	return (0);
}