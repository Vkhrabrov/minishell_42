/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utilities.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 11:12:05 by ccarrace          #+#    #+#             */
/*   Updated: 2023/12/03 11:30:30 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* is_path_null();
 *
 *	No path provided or argument is a tilde (~): go to user's home directory.
 *	Reset PWD and OLDWPD values.
*/
int	is_path_null(t_env_lst *env_lst, char *path)
{
	char	*oldpwd_value;

	oldpwd_value = ft_strdup(get_env_var_value(env_lst, "PWD"));
	path = get_env_var_value(env_lst, "HOME");
	if (path == NULL || path[0] == '\0')
	{
		free(oldpwd_value);
		return (build_error_msg("cd: ", "HOME", MS_NOTSET, false));
	}
	if (chdir(path) == 0)
	{
		update_env_var_value(env_lst, "OLDPWD", oldpwd_value);
		update_env_var_value(env_lst, "PWD", path);
	}
	else
	{
		free(oldpwd_value);
		perror("chdir");
		return (EXIT_FAILURE);
	}
	free(oldpwd_value);
	return (EXIT_SUCCESS);
}

/* are_hyphens_valid():
 *
 *	Cases: 
 *	cd - (swaps PWD and OLDPWD)
 *	cd -- (sets HOME as PWD, updates OLDPWD)
 *	invalid cases: cd --- / cd --abc ...
*/
int	are_hyphens_valid(t_env_lst *env_lst, char *path)
{
	char	*pwd_value;
	char	*oldpwd_value;

	if (!path[2])
	{
		pwd_value = ft_strdup(get_env_var_value(env_lst, "PWD"));
		oldpwd_value = ft_strdup(get_env_var_value(env_lst, "OLDPWD"));
		chdir(oldpwd_value);
		if (!path[1])
			update_env_var_value(env_lst, "PWD", oldpwd_value);
		else if (path[1] == '-')
			update_env_var_value(env_lst, "PWD", \
					get_env_var_value(env_lst, "HOME"));
		update_env_var_value(env_lst, "OLDPWD", pwd_value);
		printf("%s\n", get_env_var_value(env_lst, "PWD"));
		free(pwd_value);
		free(oldpwd_value);
		return (EXIT_SUCCESS);
	}
	else
		return (build_error_msg("cd: ", "--", MS_INVALDOPT, false));
}

/*	is_arg_valid()
 *	
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
bool	is_arg_properly_quoted(char *arg)
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
		break ;
	}
	if (opening_quotes % 2 == 0)
		return (true);
	return (false);
}

char	*get_curr_work_dir(void)
{
	char	buffer[PATH_MAX];
	char	*curr_work_dir;

	curr_work_dir = getcwd(buffer, sizeof(buffer));
	if (!curr_work_dir)
		perror("getcwd");
	return (curr_work_dir);
}
