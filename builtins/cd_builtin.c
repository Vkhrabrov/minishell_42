/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 20:19:10 by ccarrace          #+#    #+#             */
/*   Updated: 2023/11/04 01:54:13 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_curr_work_dir()
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
	char 	*error_msg;
	char	*old_pwd;

	path = args_lst[0].content;
	old_pwd = get_curr_work_dir();
	if (path == NULL) // No path provided:	go to user's home directory
	{
		path = getenv("HOME");
		if (path == NULL)
		{
			printf("HOME environment variable is not set\n");
			return (1);
		}
	}
	if (chdir(path) == 0)
	{
		while (env_lst != NULL)
		{
			if (ft_strncmp(env_lst->var_name, "OLDPWD", 6) == 0)
			{
printf("__________path stored in 'oldpwd': %s__________\n", old_pwd);
				free(env_lst->var_value);
				env_lst->var_value = ft_strdup(old_pwd);
printf("__________path dup in OLDPWD node: %s__________\n", old_pwd);
			}
			else if (ft_strncmp(env_lst->var_name, "PWD", 3) == 0)
			{
				free(env_lst->var_value);
				env_lst->var_value = ft_strdup(get_curr_work_dir());
printf("__________path dup in PWD node: %s__________\n", get_curr_work_dir());
			}
			env_lst = env_lst->next;
		}
		return (0);
	}
	else
	{
		// printf("minishell: cd: %s: No such file or directory\n", path);
		error_msg = ft_strjoin("minishell: cd: ", path);
		perror(error_msg);
		free(error_msg);
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
