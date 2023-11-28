/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 20:19:10 by ccarrace          #+#    #+#             */
/*   Updated: 2023/11/28 21:05:24 by ccarrace         ###   ########.fr       */
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
static bool is_arg_properly_quoted(char *arg)
{
    size_t i;
    size_t opening_quotes;

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
		break; //i++;
    }
	if (opening_quotes % 2 == 0)
		return (true);
	return (false);
}

static char	*get_curr_work_dir()
{
    char 	buffer[PATH_MAX];
    char	*curr_work_dir;
  
	curr_work_dir = getcwd(buffer, sizeof(buffer));
    if (!curr_work_dir)
        perror("getcwd");
	return (curr_work_dir);
}

/* is_path_null();
 *
 *	No path provided or argument is a tilde (~): go to user's home directory
*/
int	is_path_null(t_env_lst *env_lst, char *path)
{
	path = get_env_var_value(env_lst, "HOME");
	if (path == NULL || path[0] == '\0')
		return(build_error_msg("cd: ", "HOME", " not set", false));
	if (chdir(path) == 0)
	{
		update_env_var_value(env_lst, "OLDPWD", ft_strdup(get_env_var_value(env_lst, "PWD")));
		update_env_var_value(env_lst, "PWD", ft_strdup(get_curr_work_dir()));
	}
	else
	{
		perror("chdir");
		return (EXIT_FAILURE);
	}
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
			update_env_var_value(env_lst, "PWD", get_env_var_value(env_lst, "HOME"));
		update_env_var_value(env_lst, "OLDPWD", pwd_value);
		printf("%s\n", get_env_var_value(env_lst, "PWD"));
		free(pwd_value);
		free(oldpwd_value);
		return (EXIT_SUCCESS);
	}
	else
		return(build_error_msg("cd: ", "--", ": invalid option", false));
}

int update_pwd_and_oldpwd(t_env_lst *env_lst)
{
    char *oldpwd_value;
    char *pwd_value;

    oldpwd_value = ft_strdup(get_env_var_value(env_lst, "PWD"));
    pwd_value = ft_strdup(get_curr_work_dir());

    if (update_env_var_value(env_lst, "OLDPWD", oldpwd_value) == 1 ||
        update_env_var_value(env_lst, "PWD", pwd_value) == 1)
    {
        free(oldpwd_value);
        free(pwd_value);
        return (EXIT_FAILURE);
    }
    free(oldpwd_value);
    free(pwd_value);
    return (EXIT_SUCCESS);
}

int	handle_file_or_folder_errors(char *path)
{
	if (errno == ENOTDIR)
		build_error_msg("cd: ", path, ": Not a directory", false);
	else if (errno == EACCES)
		build_error_msg("cd: ", path, ": Permission denied", false);
	else
		build_error_msg("cd: ", path, ": No such file or directory", false);
	return (EXIT_FAILURE);
}

// bool	is_path_too_long(token *args_lst)
// {
// 	if (ft_strlen(args_lst->content) > 255)
// 		return (true);
// 	return (false);
// }

int	cd_builtin(t_env_lst *env_lst, token *args_lst)
{
	char	*path;
	char	*oldpwd_value;

	path = NULL;
	if (args_lst != NULL && args_lst->content != NULL)
		path = args_lst->content;
	if (path == NULL || *path == '~')
		return (is_path_null(env_lst, path));
	else if ((path[0] == '-' && !path[1]) || (path[0] == '-' && path[1] == '-'))
		return (are_hyphens_valid(env_lst, path));
	// else if (is_path_too_long(args_lst) == true)
	// 	return (build_error_msg("cd: ", path, ": File name too long", false));
	else if (ft_strlen(args_lst->content) > 255)
		return (build_error_msg("cd: ", path, ": File name too long", false));
	else if (is_arg_properly_quoted(path) == false)
		return (build_error_msg("cd :", path, ": No such file or directory", false));
	oldpwd_value = ft_strdup(get_env_var_value(env_lst, "PWD"));
	if (chdir(path) == 0)
	{
		int	result = update_pwd_and_oldpwd(env_lst);
		free(args_lst);
		return (result);
	}
	else
		return (handle_file_or_folder_errors(path));
}
