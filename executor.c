/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 23:30:07 by vkhrabro          #+#    #+#             */
/*   Updated: 2023/10/31 19:43:02 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void process_command_list(command_node *head, t_env_lst *env_lst) 
{
    int node_count = 0;
    command_node *current = head;
    // List of built-in commands (add your built-ins to this list)
    char *builtins[] = {"cd", "echo", "env", "exit", "export", "pwd", "unset", NULL}; 

    // Count the number of nodes
    while (current) {
        node_count++;
        current = current->next;
    }

    if (node_count > 1) {
        printf("Launching pipex...\n");
        return;  // Exit function
    }

    if (node_count == 1) {
        int is_builtin = 0;
        for (int i = 0; builtins[i]; i++) {
            if (ft_strncmp(head->command->content, builtins[i], ft_strlen(builtins[i])) == 0)
            {
                is_builtin = 1;
                break;
            }
        }
        if (is_builtin) {
            printf("Executing built-in command: %s\n", head->command->content);
			execute_builtin(head->command->content, head, *env_lst);
        } else {
            printf("Launching pipex execution process...\n");
        }
    }
}
