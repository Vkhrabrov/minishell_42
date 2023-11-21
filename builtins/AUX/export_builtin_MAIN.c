/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin_MAIN.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 21:00:11 by ccarrace          #+#    #+#             */
/*   Updated: 2023/11/19 10:50:39 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../minishell.h"
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>

typedef struct token token;

struct token
{
    char       *content;
    token      *next;
};

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
		substr = (char *)malloc(sizeof(char));
		if (!substr)
			return (NULL);
		substr[0] = '\0';
		return (substr);
	}
	if (start + len > strlen(s))
		len = strlen(s) - start;
	substr = (char *)malloc(sizeof(char) * (len + 1));
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

void	print_export_list(t_env_lst **env_lst)
{
	t_env_lst *current;
	
	current = *env_lst;
    while (current != NULL)
    {
char	*test_name = current->var_name;
        printf("declare -x %s", current->var_name);
		// if (current->var_value != NULL)
        // 	printf("=\"%s", current->var_value);
        // printf("\"\n");
		if (current->var_value != NULL)
{
char	*test_value = current->var_value;
        	printf("=\"%s\"\n", current->var_value);
}
        printf("\"\"\n");		
        current = current->next;       
    }
}

void	free_temp_ptr(t_env_lst *temp_ptr)
{
    t_env_lst *temp;

    while (temp_ptr != NULL)
	{
        temp = temp_ptr;
        temp_ptr = temp_ptr->next;
        free(temp->var_name);
		if (temp->var_value != NULL)
			free(temp->var_value);
        free(temp);
    }
}

int	ft_list_size(token *args_lst)
{
	int	i;

	i = 0;
	while (args_lst)
	{
		i++;
		args_lst = args_lst->next;
	}
	return (i);
}

void set_new_var(t_env_lst **head, char *arg)
{
	t_env_lst *new_node;
    size_t equal_sign_position = find_char_index(arg, '=');
    size_t arg_len = strlen(arg);


    // Search for an existing node with the same name
    while (*head != NULL)
	{
char	*test_name = (*head)->var_name;
        if (strncmp((*head)->var_name, arg, equal_sign_position) == 0 && (*head)->var_name[equal_sign_position] == '\0')
		{
            // Node with the same name exists, update its value
            if (equal_sign_position == arg_len)
			{
                // If no value provided in arg, set it to NULL
                if ((*head)->var_value != NULL)
				{
                    free((*head)->var_value);
                    (*head)->var_value = NULL;
                }
            }
			else if (strncmp((*head)->var_value, arg + equal_sign_position + 1, arg_len - equal_sign_position - 1) != 0) {
                // Update value if it's different
                free((*head)->var_value);
                (*head)->var_value = strdup(arg + equal_sign_position + 1);
            }
            return; // Node already updated, no need to create a new one
        }
        head = &(*head)->next;
    }
    // If no existing node with the same name, create a new node
    new_node = (t_env_lst *)malloc(sizeof(t_env_lst));
    if (new_node == NULL)
        return;

    if (equal_sign_position == arg_len) {
        new_node->var_name = strdup(arg);
        new_node->var_value = NULL;
    } else {
        new_node->var_name = ft_substr(arg, 0, equal_sign_position);
        new_node->var_value = strdup(arg + equal_sign_position + 1);
    }
    // new_node->next = NULL;
	new_node->next = *head;
    *head = new_node;
}

t_env_lst *insert_sorted(t_env_lst **head, t_env_lst *new_node)
{
    t_env_lst *current = *head;
    t_env_lst *prev = NULL;

    // it doesn't matter whether the length we pass to ft_strncomp is 
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
	return (*head);
}

int export_builtin(t_env_lst *env_lst, token *args_lst)
{
    t_env_lst	*sorted_list;
	t_env_lst *new_node;

    if (args_lst && ft_list_size(args_lst) > 0)
	{
        while (args_lst != NULL)
		{
            set_new_var(&env_lst, args_lst->content);
            args_lst = args_lst->next;	
        }
    }
	else
	{
        // Create a new linked list to store the sorted environment variables
        sorted_list = NULL;
        // Copy all of the environment variables to the new linked list
        while (env_lst != NULL)
		{
            new_node = (t_env_lst *)malloc(sizeof(t_env_lst));
            if (new_node == NULL)
                return (1);
            new_node->var_name = strdup(env_lst->var_name);
            new_node->var_value = strdup(env_lst->var_value);
            new_node->next = NULL;
            sorted_list = insert_sorted(&sorted_list, new_node);
            env_lst = env_lst->next;
        }
        // Print the sorted list of environment variables
        print_export_list(&sorted_list);

        // Free the sorted list of environment variables
        // while (sorted_list != NULL)
		// {
        //     t_env_lst *next_node = sorted_list->next;
        //     free(sorted_list->var_name);
		// 	if (sorted_list->var_value != NULL)
        //     	free(sorted_list->var_value);
        //     free(sorted_list);
        //     sorted_list = next_node;
        // }
    }
    return (0);
}

int	main(int argc, char **argv)
{
	int		i = 0;
	char	str_a[]="NODE_1";
	char	str_b[]="NODE_2";
	char	str_c[]="NODE_3";
	char	a[]="1";
	char	b[]="2";
	char	c[]="3";

	t_env_lst	*a_node = (t_env_lst *)malloc(sizeof(t_env_lst));
	t_env_lst	*b_node = (t_env_lst *)malloc(sizeof(t_env_lst));
	t_env_lst	*c_node = (t_env_lst *)malloc(sizeof(t_env_lst));

	a_node->next = b_node;
	b_node->next = c_node;
	c_node->next = NULL;

	a_node->var_name = str_a;
	a_node->var_value = a;
	b_node->var_name = str_b;
	b_node->var_value = b;
	c_node->var_name = str_c;
	c_node->var_value = c;

	t_env_lst *lst;
	lst = a_node;

	token	*new_arg;
	token	*current = NULL;
	token	*args_lst = NULL;
	while (i < argc)
	{
		new_arg = (token *)malloc(sizeof(token));
		new_arg->content = argv[i];
		new_arg->next = NULL;
		args_lst = new_arg;
		if (i == 0)
			current = new_arg;
		else
		{
			current->next = new_arg;
			current = new_arg;
		}
		i++;
	} 
	// while (lst)
	// {
	// 	printf("%s\n", lst->var_name);
	// 	lst = lst->next;
	// }

	export_builtin(lst, args_lst);
}