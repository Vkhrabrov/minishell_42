/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkhrabro <vkhrabro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 23:30:07 by vkhrabro          #+#    #+#             */
/*   Updated: 2023/10/26 21:01:19 by vkhrabro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void process_command_list(command_node *head) 
{
    int node_count = 0;
    command_node *current = head;
    // List of built-in commands (add your built-ins to this list)
    char *builtins[] = {"cd", "echo", "exit", "env", "setenv", "unsetenv", NULL}; 

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
            if (ft_strncmp(head->command->content, builtins[i], ft_strlen(head->command->content)) == 0)
            {
                is_builtin = 1;
                break;
            }
        }
        if (is_builtin) {
            printf("Executing built-in command: %s\n", head->command->content);
        } else {
            printf("Launching pipex execution process...\n");
        }
    }
}
