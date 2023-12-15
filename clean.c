/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 21:01:39 by ccarrace          #+#    #+#             */
/*   Updated: 2023/12/15 00:41:58 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	free_token(token *t)
{
    token *current;

    while (t != NULL)
	{
        current = t;
        free(current->content);
		free(current);
        t = t->next;
    }	
}

void	free_command_node(command_node *cmd_node)
{
    if (cmd_node == NULL)
        return ;

    free_token(cmd_node->command);
    // free_token(cmd_node->args);
    free_token(cmd_node->redirect_in);
    free_token(cmd_node->redirect_out);
    free_token(cmd_node->redirect_append);
	free_token(cmd_node->redirections);
    free_token(cmd_node->var_expansion);
    free_token(cmd_node->env_variable);
    free_token(cmd_node->ex_status);

    // Free other allocated strings
    	free(cmd_node->redirect_in_filename);
    	free(cmd_node->redirect_out_filename);
    	free(cmd_node->here_doc_content);

    // Finally, free the current command_node
    free(cmd_node);
}

// void	free_command_node(command_node *cmd_node)
// {
//     if (cmd_node == NULL)
//         return ;
// 	if (cmd_node->command != NULL)
//     	free_token(cmd_node->command);
// 	if (cmd_node->args != NULL)	
//     free_token(cmd_node->args);
// 	if (cmd_node->redirect_in != NULL)
//     	free_token(cmd_node->redirect_in);
// 	if (cmd_node->redirect_out != NULL)
//     	free_token(cmd_node->redirect_out);
// 	if (cmd_node->redirect_append != NULL)
//     	free_token(cmd_node->redirect_append);
//     if (cmd_node->redirections != NULL)
// 		free_token(cmd_node->redirections);
// 	if (cmd_node->var_expansion != NULL)
//     	free_token(cmd_node->var_expansion);
// 	if (cmd_node->env_variable != NULL)
//     	free_token(cmd_node->env_variable);
// 	if (cmd_node->ex_status != NULL)
//     	free_token(cmd_node->ex_status);

//     // Free other allocated strings
// 	if (cmd_node->redirect_in != NULL)
//     	free(cmd_node->redirect_in_filename);
// 	if (cmd_node->redirect_in != NULL)
//     	free(cmd_node->redirect_out_filename);
// 	if (cmd_node->redirect_in != NULL)
//     	free(cmd_node->here_doc_content);

//     // Finally, free the current command_node
//     free(cmd_node);
// }