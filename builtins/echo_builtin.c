/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 22:11:13 by ccarrace          #+#    #+#             */
/*   Updated: 2023/11/12 13:13:54 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// gcc echo_builtin.c ../libft/ft_strncmp.c ../libft/ft_lstsize.c ../libft/ft_strlen.c -I ../libft -o echo

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

void print_list(token *args_lst)
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
	bool	is_valid_option;

	is_valid_option = false;
	// print_list(args_lst);
	if (!args_lst)
		printf("\n");
	else
	{
		// while (ft_strncmp(args_lst->content, "-n", find_max_len(args_lst->content, "-n")) == 0)
		while (args_lst && ft_strncmp(args_lst->content, "-n", 2) == 0)
		{
			// check if option -n is valid or not
			if (args_lst->content[2] && args_lst->content[2] != 'n')
				break;
			else
				is_valid_option = true;
			args_lst = args_lst->next;
		}
		while (args_lst != NULL)
		{
			if (is_valid_option == true) // valid -n option, print args close to prompt (no newline)
				printf("%s", args_lst->content);
			else
			{
				printf("%s", args_lst->content); // no valid -n option, print all args
				if (args_lst->next != NULL)	  // print a space between args, but not after the last one
					printf(" ");
			}
			args_lst = args_lst->next;
		}
		if (is_valid_option == false) // if no valid -n option found, newline after the last one has been printed
			printf("\n");
	}
	return (0);
}


// int	main(int argc, char **argv)
// {
// 	token	*new_node;
// 	token	*args_lst;
// 	token	*head;
// 	token	*current;
// 	token	*next;
// 	int		i;

// 	args_lst = NULL;
// 	head = args_lst;
// 	i = 1;
// 	if (argc == 1)
// 		printf("No args!\n");
// 	else
// 	{
// 		while (i < argc)
// 		{
// 			new_node = malloc(sizeof(token));
// 			if (!new_node)
// 				new_node = NULL;
// 			new_node->content = argv[i];
// 			new_node->next = head;
// 			head = new_node;
// 			i++;
// 		}
// 	}
// 	current = head;
// 	while (current != NULL)
// 	{
// 		next = current->next;
// 		free(current);
// 		current = next;
// 	}
// 	return (0);
// }
