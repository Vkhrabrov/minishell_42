#include <stdbool.h>
#include <stdio.h>

size_t ft_strlen(const char *s)
{
    size_t len = 0;
    while (s[len] != '\0') {
        len++;
    }
    return len;
}

static bool is_arg_valid(char *arg)
{
    size_t i = 0;
    size_t quote_count = 0;

    printf("arg = %s\n", arg);

    while (i < ft_strlen(arg))
    {
        if (arg[i] == '"')
        {
            quote_count++;
            i++;

            // Skip until the closing double quote or end of string
            while (arg[i] != '\0' && arg[i] != '"')
                i++;

            // If we found a closing double quote, move past it
            if (arg[i] == '"')
                i++;
        }
        else
            i++;
    }

    printf("quote_count = %zu\n", quote_count);

    if (quote_count % 2 == 0)
    {
        if (quote_count == 0 || (quote_count == 2 && arg[0] == ' ' && arg[ft_strlen(arg) - 1] == ' '))
        {
            printf("bash: cd:  : No such file or directory\n");
            return false;
        }
    }

    printf("\n");
    return true;
}

int main(int argc, char **argv)
{
    (void)argc;
    if (is_arg_valid(argv[1]) == true)
        printf("bash: cd:  : No such file or directory\n");

    return 0;
}



// int main(int argc, char **argv)
// {
//     (void)argc;
//     if (is_arg_valid(argv[1]) == true)
//         printf("arg has an odd number of opening quotes\n");
//     else
//         printf("arg has an even number of opening quotes\n");

//     return 0;
// }

// static bool	is_arg_valid(char *arg)
// {
// 	size_t	i;

// 	i = 0;
// 	while (arg[i] != '\0')
// 	{
// 		if (arg[i] == '"')
// 		{
// 			printf("Opening double quote found\n");
// 			while (arg[i] == '"')
// 				i++;
// 		}
// 		else if (arg[i] == '\'')
// 		{
// 			printf("Opening single quote found\n");
// 			while (arg[i] == '\'')
// 				i++;
// 		}
// 		else
// 		{
// 			printf("arg is not quoted\n");
// 			break;
// 		}
// 	}
// printf("i = %zu\n", i);
// 	return (i % 2 != 0);
// }

// int	main(int argc, char **argv)
// {
// 	(void)argc;
// 	if (is_arg_valid(*argv) == true)
// 		printf("arg has odd number of opening quotes\n");
// 	else
// 		printf("arg has even number of opening quotes\n");
// 	return (0);
// }

// #include <stdbool.h>
// #include <stdio.h>

// size_t	ft_strlen(const char *s)
// {
// 	size_t len = 0;
// 	while (s[len] != '\0') {
// 		len++;
// 	}
// 	return len;
// }

// static bool is_arg_valid(char *arg)
// {
// 	size_t i = 0;
// 	size_t	quote_count = 0;

// printf("arg = %s\n", arg);
// 	while (i < ft_strlen(arg))
// 	{
// 		if (arg[i] == '"')
// 		{
// 			printf("Opening double quote found\n");
// 			i++;
// 			quote_count++;
// 			// while (arg[i] != '\0' && arg[i] != '"')
// 			// 	i++;
// 			while (arg[i] == '"' && arg[i] != '\0')
// 			{
// 				i++; // Move past the closing double quote
// 				quote_count++;
// 			}
// 		}
// 		else if (arg[i] == '\'')
// 		{
// 			printf("Opening single quote found\n");
// 			i++;
// 			quote_count++;
// 			// while (arg[i] != '\0' && arg[i] != '\'')
// 			// 	i++;
// 			while (arg[i] == '\'' && arg[i] != '\0')
// 			{
// 				i++; // Move past the closing single quote
// 				quote_count++;
// 			}
// 		}
// 		// else
// 		// 	break;
// 		i++;
// 	}

// 	printf("i = %zu\n", i);
// 	printf("quote_count = %zu\n", quote_count);

// 	// return (i % 2 != 0);
// 	return (i);
// }

// int	main(int argc, char **argv)
// {
// 	(void)argc;
// 	printf("i = %d\n", is_arg_valid(argv[1]));
// }

// int main() {
// 	char str1[] = "abc";
// 	char str2[] = "'abc'";
// 	char str3[] = "''abc''";
// 	char str4[] = "\"abc\"";
// 	char str5[] = "\"abc'";

// 	printf("str1: %d\n", is_arg_valid(str1)); // 0
// 	printf("str2: %d\n", is_arg_valid(str2)); // 1
// 	printf("str3: %d\n", is_arg_valid(str3)); // 0
// 	printf("str4: %d\n", is_arg_valid(str4)); // 0
// 	printf("str5: %d\n", is_arg_valid(str5)); // 1

// 	return 0;
// }
