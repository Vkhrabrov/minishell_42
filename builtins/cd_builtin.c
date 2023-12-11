/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkhrabro <vkhrabro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 20:19:10 by ccarrace          #+#    #+#             */
/*   Updated: 2023/12/11 23:23:27 by vkhrabro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_file_or_folder_errors(char *path)
{
	if (errno == ENOTDIR)
		build_error_msg("cd: ", path, MS_NOTDIR, false);
	else if (errno == EACCES)
		build_error_msg("cd: ", path, MS_ACCESFORB, false);
	else if (*path == '\0')
		return (EXIT_SUCCESS);
	else 
		build_error_msg("cd: ", path, MS_NOFILEDIR, false);
	return (EXIT_FAILURE);
}

int	update_pwd_and_oldpwd(t_env_lst *env_lst, token *args_lst, char *path)
{
	char	*oldpwd_value;
	char	*pwd_value;

	if (chdir(path) == 0)
	{
		oldpwd_value = ft_strdup(get_env_var_value(env_lst, "PWD"));
		if (get_curr_work_dir() == NULL)
			pwd_value = NULL;
		else
			pwd_value = ft_strdup(get_curr_work_dir());
		if (oldpwd_value == NULL || pwd_value == NULL)
			return (EXIT_FAILURE);
		update_env_var_value(env_lst, "OLDPWD", oldpwd_value);
		update_env_var_value(env_lst, "PWD", pwd_value);
		free(oldpwd_value);
		free(pwd_value);
		free(args_lst);
		return (EXIT_SUCCESS);
	}
	else
		return (handle_file_or_folder_errors(path));
}

// int	update_pwd_and_oldpwd(t_env_lst *env_lst, token *args_lst, char *path)
// {
// 	char	*oldpwd_value;
// 	char	*pwd_value;

// 	if (chdir(path) == 0)
// 	{
// 		oldpwd_value = ft_strdup(get_env_var_value(env_lst, "PWD"));
// 		pwd_value = ft_strdup(get_curr_work_dir());
// 		if (oldpwd_value == NULL || pwd_value == NULL)
// 			return (EXIT_FAILURE);
// 		update_env_var_value(env_lst, "OLDPWD", oldpwd_value);
// 		update_env_var_value(env_lst, "PWD", pwd_value);
// 		free(oldpwd_value);
// 		free(pwd_value);
// 		free(args_lst);
// 		return (EXIT_SUCCESS);
// 	}
// 	else
// 		return (handle_file_or_folder_errors(path));
// }

int	cd_builtin(t_env_lst *env_lst, token *args_lst)
{
	char	*path;

	path = NULL;
	if (args_lst != NULL && args_lst->content != NULL)
		path = args_lst->content;
	if (path == NULL || *path == '~')
		return (is_path_null(env_lst, path));
	else if ((path[0] == '-' && !path[1])
		|| (path[0] == '-' && path[1] == '-'))
		return (are_hyphens_valid(env_lst, path));
	else if (ft_strlen(args_lst->content) > 255)
		return (build_error_msg("cd: ", path, MS_LONGNAME, false));
	else if (is_arg_properly_quoted(path) == false)
		return (build_error_msg("cd :", path, MS_NOFILEDIR, false));
	return (update_pwd_and_oldpwd(env_lst, args_lst, path));
}
