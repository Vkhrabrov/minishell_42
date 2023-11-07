/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list_creation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 20:53:39 by vkhrabro          #+#    #+#             */
/*   Updated: 2023/11/08 00:11:36 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void t_env_init( t_env_lst  *env_lst)
// {
//     env_lst->var_name = NULL;
//     env_lst->var_value = NULL;
// }

size_t find_char_index(const char *str, char target) 
{
    size_t index;
    
    index = 0;
    while (str[index] != '\0' && str[index] != target)
        index++;
    return index;
}

void add_env_var_to_list(t_env_lst **head, const char *envp_line)
{
    t_env_lst	*new_node;
    size_t		equal_sign_position;
    size_t		envp_line_len;

    new_node = malloc(sizeof(t_env_lst));
    if (new_node == NULL)
		return ;
    equal_sign_position = find_char_index(envp_line, '=');
    envp_line_len = ft_strlen(envp_line);
    if (equal_sign_position == envp_line_len)
	{
        new_node->var_name = ft_strdup(envp_line);
        new_node->var_value = NULL;
    }
	else
	{
        new_node->var_name = ft_substr(envp_line, 0, equal_sign_position);
		if (ft_strncmp(new_node->var_name, "OLDPWD", 6) == 0)
        	new_node->var_value = NULL;		
		else	
        	new_node->var_value = ft_substr(envp_line, equal_sign_position + 1, envp_line_len - equal_sign_position);
    }
	new_node->next = NULL;
  	if (*head == NULL)
        *head = new_node;
	else
	{
		t_env_lst	*current = *head;
		while (current->next != NULL)
			current = current->next;
		current->next = new_node;
	}
}

void	save_env_list(t_env_lst **env_lst, char **envp)
{
	int	i;
	i = 0;
    while (envp[i] != NULL)
    {
        add_env_var_to_list(env_lst, envp[i]);
        i++;       
    }
}

void	print_env_list(t_env_lst **env_lst)
{
	t_env_lst *current;
	
	current = *env_lst;
    while (current != NULL)
    {
        printf("%s=%s\n", current->var_name, current->var_value);
        current = current->next;       
    }
}

void	free_env_list(t_env_lst *env_lst)
{
    t_env_lst *temp;

    while (env_lst != NULL)
	{
        temp = env_lst;
        env_lst = env_lst->next;
        free(temp->var_name);
		if (temp->var_value != NULL)
			free(temp->var_value);
        free(temp);
    }
}