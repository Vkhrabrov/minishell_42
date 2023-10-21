/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 21:00:11 by ccarrace          #+#    #+#             */
/*   Updated: 2023/10/21 21:51:29 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>      // printf()
#include <string.h>     // strchr(), strdup(), strlen(), strncmp()
#include <stdlib.h>     // malloc()

typedef struct  s_env_lst
{
    char				*var_name;
    char				*var_value;
	struct s_env_lst	*next;
}
t_env_lst;

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*substr;

	if (!s)
		return (NULL);
	if (strlen(s) < start)
	{
		substr = malloc(sizeof(char));
		if (!substr)
			return (NULL);
		substr[0] = '\0';
		return (substr);
	}
	if (start + len > strlen(s))
		len = strlen(s) - start;
	substr = malloc(sizeof(char) * (len + 1));
	if (!substr)
		return (NULL);
	i = 0;
	while (i < len)
		substr[i++] = s[start++];
	substr[i] = '\0';
	return (substr);
}

size_t find_char_index(const char *str, char target) 
{
    size_t index;
    
    index = 0;
    while (str[index] != '\0' && str[index] != target)
        index++;
    return index;
}

void insert_sorted(t_env_lst **head, t_env_lst *new_node)
{
    t_env_lst *current = *head;
    t_env_lst *prev = NULL;

    // it doesn't matter wheter the length we pass to ft_strncomp is 
	// 'strlen(current->var_name)' or 'strlen(new_node->var_name)'
	while (current != NULL && strncmp(new_node->var_name, current->var_name, strlen(current->var_name)) > 0)
	{
        prev = current;
        current = current->next;
    }
    if (prev == NULL)
	{
        // Insert at the beginning
        new_node->next = *head;
        *head = new_node;
    }
	else
	{
        // Insert in the middle or at the end
        prev->next = new_node;
        new_node->next = current;
    }
}

void add_env_var_to_list(t_env_lst **head, const char *envp_line)
{
    t_env_lst *new_node;
    size_t equal_sign_position;
    size_t envp_line_len;

    new_node = malloc(sizeof(t_env_lst));
    if (new_node == NULL) 
        exit(-1);
    equal_sign_position = find_char_index(envp_line, '=');
    envp_line_len = strlen(envp_line);
    if (equal_sign_position == envp_line_len)
	{
        new_node->var_name = strdup(envp_line);
        new_node->var_value = NULL;
    }
	else
	{
        new_node->var_name = ft_substr(envp_line, 0, equal_sign_position);
        new_node->var_value = ft_substr(envp_line, equal_sign_position + 1, envp_line_len - equal_sign_position);
    }
    insert_sorted(head, new_node);
}

void	fill_env_list(t_env_lst **env_lst, char *envp[])
{
	int	i;

	i = 0;
    while (envp[i] != NULL)
    {
        add_env_var_to_list(env_lst, envp[i]);
        i++;       
    }
}

void export(char *envp[], char *new_var) 
{
    t_env_lst   *env_lst;
    int i;

    env_lst = NULL;
    i = 0;
    if (new_var == NULL)
        fill_env_list(&env_lst, envp);
    else
    {
        fill_env_list(&env_lst, envp);
        add_env_var_to_list(&env_lst, new_var);
    }
    while (env_lst != NULL)
    {
        printf("declare -x %s", env_lst->var_name);
        printf("=\"%s", env_lst->var_value);
        printf("\"\n");
        env_lst = env_lst->next;       
    }
}

int main(int argc, char **argv, char *envp[]) 
{
    (void)argc;

    if (argv[1] == NULL)
        export(envp, NULL);
    else
        export(envp, argv[1]);
    return (0);
}
