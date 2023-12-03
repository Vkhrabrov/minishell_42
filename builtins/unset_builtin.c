/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 20:29:41 by ccarrace          #+#    #+#             */
/*   Updated: 2023/12/03 14:01:28 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* remove_node()
 *
 *	- if node to remove is in first place, set head of the list to next one
 *	- else, link previous node of the one we need to remove to its next node
*/

void	remove_node(t_env_lst **env_lst, t_env_lst *current, t_env_lst *prev)
{
	if (prev == NULL)
		*env_lst = current->next;
	else
		prev->next = current->next;
	free(current->var_name);
	free(current->var_value);
	free(current);
}

/*
 * We pass a pointer to the pointer of the head of the list because we
 * may need to modify the head of the list if the first node matches.
 * If we just pass a pointer to the list (t_env_lst *env_lst), we won't 
 * be able to update the head of the list in case we remove the first node. 
*/
int	unset_builtin(t_env_lst **env_lst, token *args_lst)
{
	t_env_lst	*current;
	t_env_lst	*previous;
	size_t		equal_sign_position;

	equal_sign_position = find_char_index(args_lst->content, '=');
	if (!is_var_name_valid(args_lst->content, equal_sign_position))
		return (build_error_msg("unset: ", args_lst->content, MS_BADID, true));
	while (args_lst != NULL)
	{
		current = *env_lst;
		previous = NULL;
		while (current != NULL)
		{
			if (ft_strncmp(args_lst->content, current->var_name, \
				ft_strlen(args_lst->content)) == 0)
				remove_node(env_lst, current, previous);
			else
			{
				previous = current;
				current = current->next;
			}
		}
		args_lst = args_lst->next;
	}
	return (EXIT_SUCCESS);
}
