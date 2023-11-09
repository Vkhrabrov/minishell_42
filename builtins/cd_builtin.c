/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 20:19:10 by ccarrace          #+#    #+#             */
/*   Updated: 2023/11/09 22:37:48 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


/* 
 *	is_arg_valid()
 *	- odd number of single/double quotes (e.g. " " or """ """)
 *		Bash interprets it as an attempt to change to a directory with a single
 *		space in its name. If there is no directory with that name, you get the 
 *		"No such file or directory" error.
 *	- even number of single/double quotes (e.g "" "" or """" """")
 *		We are providing two empty strings as arguments. Bash interprets this as
 *		an attempt to change to a directory with an empty string as its name. 
 *		Since an empty string is a valid directory name, no error is returned,
 *		and cd successfully changes to the current working directory.
*/
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
// 			break;
// 	}
// printf("i = %zu\n", i);
// 	return (i % 2 != 0);
// }

static bool	is_arg_valid(char *arg)
{
	size_t	i;
	size_t	opening_quotes;

	i = 0;
	opening_quotes = 0;
	while (i < ft_strlen(arg))
	{
		if (arg[i] == '"')
		{
			opening_quotes++;
			while (arg[i] == '"')
				i++;
		}
		else if (arg[i] == '\'')
		{
			opening_quotes++;
			while (arg[i] == '\'')
				i++;
		}
		else
			break;
	}
printf("i = %zu\n", i);
printf("opening_quotes = %zu\n", i);
	if (opening_quotes % 2 == 0)
		return (false);
	else
		return (true);
}


char	*get_curr_work_dir()
{
    char 	buffer[PATH_MAX];
    char	*curr_work_dir;
  
	curr_work_dir = getcwd(buffer, sizeof(buffer));
    if (!curr_work_dir)
        perror("getcwd");
	return (curr_work_dir);
}

char	*get_env_var_value(t_env_lst *env_lst, char *str)
{
	while (env_lst != NULL)
	{
		if (ft_strncmp(env_lst->var_name, str, find_max_len(env_lst->var_name, str)) == 0)
			return(env_lst->var_value);
		env_lst = env_lst->next;
	}
	return (0);
}

int	cd_builtin(t_env_lst *env_lst, token *args_lst)
{
	char	*path;
	char	*old_pwd;

	path = NULL;
	if (args_lst != NULL)
		path = args_lst->content;
	if (is_arg_valid(path) == false)
		return (build_error_msg("cd :", path, ": No such file or directory", false ));
	old_pwd = ft_strdup(get_curr_work_dir());
	if (path == NULL) // No path provided:	go to user's home directory
	{
		path = get_env_var_value(env_lst, "HOME");
		if (path == NULL || path[0] == '\0')
		{
			build_error_msg("cd: ", "HOME", " not set", false);
			return (1);
		}
	}
	if (chdir(path) == 0)
	{
		while (env_lst != NULL)
		{
			if (ft_strncmp(env_lst->var_name, "OLDPWD", 6) == 0)
			{
				free(env_lst->var_value);
				env_lst->var_value = ft_strdup(old_pwd);
			}
			else if (ft_strncmp(env_lst->var_name, "PWD", 3) == 0)
			{
				free(env_lst->var_value);
				env_lst->var_value = ft_strdup(get_curr_work_dir());
			}
			env_lst = env_lst->next;
		}
		free(old_pwd);
		free(args_lst);
		return (0);
	}
	else
	{
		build_error_msg("cd: ", path, ": No such file or directory", false);
		return (1);
	}
}

// int	main(int argc, char **argv)
// {
// 	if (argc == 1)
// 		return cd_builtin(NULL);
// 	else if (argc == 2)
// 		return cd_builtin(argv[1]);
// 	{
// 		ret
// 	}
// }
