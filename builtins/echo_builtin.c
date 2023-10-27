/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 22:11:13 by ccarrace          #+#    #+#             */
/*   Updated: 2023/10/27 22:18:34 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// gcc echo_builtin.c ../libft/ft_strncmp.c ../libft/ft_lstsize.c -I ../libft -o echo

#include "../minishell.h"
#include <stdio.h>
#include <string.h>		//	strncmp(), strlen() --> REPLACE with libf fts!!!!!
#include <stdbool.h>
#include "../libft/libft.h"

typedef	struct	s_node
{
	char			*content;
	struct s_node	*next;
}	t_node;

t_node	*ft_create_node(char *str)
{
	t_node	*node;

	node = malloc(sizeof(t_node));
	if (!node)
		return (NULL);
	node->content = str;
	node->next = NULL;
	return (node);
}

void	ft_add_to_back(t_node **lst, char *str)
{
	t_node	*old_tail;
	t_node	*new_tail;
	t_node	*current;

	if (!*lst)
		*lst = ft_create_node(str);
	else
	{
		current = *lst;
		new_tail = ft_create_node(str);
		while (current->next)
			current = current->next;
		old_tail = current;
		old_tail->next = new_tail;
	}
}

void	create_args_lst(t_node **args_lst, int argc, char **argv)
{
	int		i;

	if (argc == 1)
		*args_lst = NULL;
	else
	{
		i = 1;
		while (i < argc)
		{
			ft_add_to_back(args_lst, argv[i]);
			i++;
		}
	}
}

void	print_list(token *args_lst)
{
printf("entered print_list\n");
	while (args_lst != NULL)
	{
		printf("%s\n", args_lst->content);
		args_lst = args_lst->next;
	}
}

int	echo_builtin(token *args_lst)
{
	bool	option_passed;

	option_passed = false;
	// print_list(args_lst);
	if (!args_lst)
		printf("\n");
	else
	{
		while (ft_strncmp(args_lst->content, "-n", 2) == 0)
		{
			args_lst = args_lst->next;
			option_passed = true;
		}
		while (args_lst != NULL)
		{
			printf("%s", args_lst->content);
			if (args_lst->next != NULL)
				printf(" ");
			args_lst = args_lst->next;
		}
		if (option_passed == false)
			printf("\n");
	}
	return (0);
}
