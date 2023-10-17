/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkhrabro <vkhrabro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 23:30:07 by vkhrabro          #+#    #+#             */
/*   Updated: 2023/10/16 00:02:06 by vkhrabro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char **convert_node_to_args(command_node *node)
{
    int num_args = 1; // For the command itself.
    token *arg = node->args;
    while (arg)
    {
        num_args++;
        arg = arg->next;
    }

    char **args = malloc(sizeof(char *) * (num_args + 1)); // +1 for the NULL terminator.
    int i = 0;

    args[i++] = node->command->content;
    arg = node->args;
    while (arg)
    {
        args[i++] = arg->content;
        arg = arg->next;
    }
    args[i] = NULL; // Terminating the args array.

    return args;
}


void execute_single_command(t_pipex *tab, command_node *node, char **envp)
{
    check_path(tab, node->command->content, envp);
    char **args = convert_node_to_args(node);
    if (execve(tab->cmd, args, envp) == -1)
    {
        // Handle the error appropriately.
        perror("Error executing command");
        exit(clean_exit(tab, 1));
    }
}

t_pipex *initialize_pipex_from_parsed(command_node *parsed_cmds)
{
    t_pipex *tab = malloc(sizeof(t_pipex));
    // Initialize any other fields in the t_pipex structure if necessary.
    tab->in_fd = STDIN_FILENO; // Defaulting to standard input for now.
    tab->out_fd = STDOUT_FILENO; // Defaulting to standard output for now.
    tab->b = 2; // Assuming the default value, adjust as necessary.
    tab->heredoc = 0; // Assuming no heredoc for now, adjust as necessary.

    // If your parser supports here_doc and other features, you'll need to
    // add more logic here to set the appropriate fields in the t_pipex structure.

    return tab;
}

void execute_from_parsed(command_node *parsed_cmds, char **envp)
{
    t_pipex *tab = initialize_pipex_from_parsed(parsed_cmds);

    if (!parsed_cmds->next) // This means there's only one command_node (no pipe).
    {
        execute_single_command(tab, parsed_cmds, envp);
    }
    else
    {
        command_node *current = parsed_cmds;
        int argc = 2; // Starting at 2 to fit the pipex format.
        while (current)
        {
            char **args = convert_node_to_args(current);
            pipex(tab, argc, args, envp); // Passing the arguments to pipex.
            current = current->next;
            argc++; // Increase argc as you loop through the commands.
        }
    }

    // Remember to free the dynamically allocated memory.
    free(tab);
}
