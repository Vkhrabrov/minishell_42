/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 20:19:10 by ccarrace          #+#    #+#             */
/*   Updated: 2023/11/20 23:31:36 by ccarrace         ###   ########.fr       */
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
static bool is_arg_valid(char *arg)
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

int	cd_builtin(t_env_lst *env_lst, token *args_lst)
{
	char	*path;
	char	*pwd_value;
	char	*oldpwd_value;
	t_env_lst	*temp_ptr;

	path = NULL;
	if (args_lst != NULL && args_lst->content != NULL)
		path = args_lst->content;
	if (path == NULL || *path == '~') // No path provided:	go to user's home directory
	{
		path = get_env_var_value(env_lst, "HOME");
		if (path == NULL || path[0] == '\0')
			return(build_error_msg("cd: ", "HOME", " not set", false));
	}
	// Case: cd - or cd -- (swaps PWD and OLDPWD) / invalid cases: cd --- / cd --abc ...
	else if ((path[0] == '-' && !path[1]) || (path[0] == '-' && path[1] == '-'))
	{
		if (!path[2])
		{
			pwd_value = ft_strdup(get_env_var_value(env_lst, "PWD"));
			oldpwd_value = ft_strdup(get_env_var_value(env_lst, "OLDPWD"));
			chdir(oldpwd_value);
			temp_ptr = env_lst;
			while (temp_ptr)
			{
				if (ft_strncmp(temp_ptr->var_name, "PWD", find_max_len(temp_ptr->var_name, "PWD")) == 0)
				{
					free(temp_ptr->var_value);
					temp_ptr->var_value = ft_strdup(oldpwd_value);
				}
				else if (ft_strncmp(temp_ptr->var_name, "OLDPWD", find_max_len(temp_ptr->var_name, "OLDPWD")) == 0)
				{
					free(temp_ptr->var_value);
					temp_ptr->var_value = ft_strdup(pwd_value);
				}
				temp_ptr = temp_ptr->next;
			}
			printf("%s\n", get_env_var_value(env_lst, "PWD"));
			free(pwd_value);
			free(oldpwd_value);
			return (0);
		}
		else
			return(build_error_msg("cd: ", "--", ": invalid option", false));
	}
	// Case: path longer than 255 characters
	else if (ft_strlen(args_lst->content) > 255)
		return(build_error_msg("cd: ", path, ": File name too long", false));
	// Case: unknown path
	else if (is_arg_valid(path) == false)
		return (build_error_msg("cd :", path, ": No such file or directory", false ));
	oldpwd_value = ft_strdup(get_env_var_value(env_lst, "PWD"));
	// Case: valid path, or no path provided (but we have set it to HOME in first condition)
	if (chdir(path) == 0)
	{
		while (env_lst != NULL)
		{
			if (ft_strncmp(env_lst->var_name, "OLDPWD", find_max_len(env_lst->var_name, "OLDPWD")) == 0)
			{
				free(env_lst->var_value);
				env_lst->var_value = ft_strdup(oldpwd_value);
			}
			else if (ft_strncmp(env_lst->var_name, "PWD", find_max_len(env_lst->var_name, "PWD")) == 0)
			{
				free(env_lst->var_value);
				env_lst->var_value = ft_strdup(get_curr_work_dir());				
			}
			env_lst = env_lst->next;
		}
		free(oldpwd_value);
		free(args_lst);
		return (0);
	}
	else
	{
		if (errno == ENOTDIR)
			build_error_msg("cd: ", path, ": Not a directory", false);
		else if (errno == EACCES)
			build_error_msg("cd: ", path, ": Permission denied", false);
		else
			build_error_msg("cd: ", path, ": No such file or directory", false);
		return (1);
	}
}

// int	cd_builtin(t_env_lst *env_lst, token *args_lst)
// {
// 	char	*path;
// 	char	*pwd_value;
// 	char	*oldpwd_value;
// 	// t_env_lst	*temp_ptr;

// 	path = NULL;
// 	if (args_lst != NULL && args_lst->content != NULL)
// 		path = args_lst->content;
// 	if (path == NULL || *path == '~') // No path provided:	go to user's home directory
// 	{
// 		path = get_env_var_value(env_lst, "HOME");
// 		if (path == NULL || path[0] == '\0')
// 			return(build_error_msg("cd: ", "HOME", " not set", false));
// 	}
// 	// Case: cd - or cd -- (swaps PWD and OLDPWD) / invalid cases: cd --- / cd --abc ...
// 	else if ((path[0] == '-' && !path[1]) || (path[0] == '-' && path[1] == '-'))
// 	{
// 		if (!path[2])
// 		{
// 			pwd_value = ft_strdup(get_env_var_value(env_lst, "PWD"));
// 			oldpwd_value = ft_strdup(get_env_var_value(env_lst, "OLDPWD"));
// 			update_env_var_value(env_lst, "OLDPWD", pwd_value);
// 			update_env_var_value(env_lst, "PWD", oldpwd_value);
// 			printf("%s\n", get_env_var_value(env_lst, "PWD"));
// 			return (0);
// 		}
// 		else
// 			return(build_error_msg("cd: ", "--", ": invalid option", false));
// 	}
// 	// Case: path longer than 255 characters
// 	else if (ft_strlen(args_lst->content) > 255)
// 		return(build_error_msg("cd: ", path, ": File name too long", false));
// 	// Case: unknown path
// 	else if (is_arg_valid(path) == false)
// 		return (build_error_msg("cd :", path, ": No such file or directory", false ));
// 	oldpwd_value = ft_strdup(get_env_var_value(env_lst, "PWD"));
// 	// Case: valid path or no path (set to HOME in first condition)
// 	if (chdir(path) == 0)
// 	{
// 		pwd_value = ft_strdup(get_env_var_value(env_lst, "PWD"));
// 		oldpwd_value = ft_strdup(get_env_var_value(env_lst, "OLDPWD"));
// 		update_env_var_value(env_lst, "OLDPWD", oldpwd_value);
// 		update_env_var_value(env_lst, "PWD", oldpwd_value);
// 		// while (env_lst != NULL)
// 		// {
// 		// 	if (ft_strncmp(env_lst->var_name, "OLDPWD", find_max_len(env_lst->var_name, "OLDPWD")) == 0)
// 		// 	{
// 		// 		free(env_lst->var_value);
// 		// 		env_lst->var_value = ft_strdup(oldpwd_value);
// 		// 	}
// 		// 	else if (ft_strncmp(env_lst->var_name, "PWD", find_max_len(env_lst->var_name, "PWD")) == 0)
// 		// 	{
// 		// 		free(env_lst->var_value);
// 		// 		env_lst->var_value = ft_strdup(get_curr_work_dir());				
// 		// 	}
// 		// 	env_lst = env_lst->next;
// 		// }
// 		// free(oldpwd_value);
// 		// free(args_lst);
// 		return (0);
// 	}
// 	else
// 	{
// 		if (errno == ENOTDIR)
// 			build_error_msg("cd: ", path, ": Not a directory", false);
// 		else if (errno == EACCES)
// 			build_error_msg("cd: ", path, ": Permission denied", false);
// 		else
// 			build_error_msg("cd: ", path, ": No such file or directory", false);
// 		return (1);
// 	}
// }
