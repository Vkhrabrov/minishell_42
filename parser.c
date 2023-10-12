/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vadimhrabrov <vadimhrabrov@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 17:44:14 by vadimhrabro       #+#    #+#             */
/*   Updated: 2023/10/12 17:45:57 by vadimhrabro      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

command_node* parse_command(token **tokens) {
    if (!tokens || !(*tokens)) return NULL;

    command_node *cmd_node = malloc(sizeof(command_node));
    cmd_node->command = NULL;
    cmd_node->args = NULL;
    cmd_node->redirects = NULL;
    cmd_node->next = NULL;

    token *current = *tokens;

    while (current && current->type != TOKEN_PIPE) {
        if (current->type == TOKEN_COMMAND) {
            cmd_node->command = current;  // Assuming only one token for command.
            current = current->next;
        } else if (current->type == TOKEN_ARGUMENT) {
            cmd_node->args = current; // For simplicity, only handling last argument. You should use a list or array.
            current = current->next;
        } else if (current->type == TOKEN_REDIRECT_OUT || current->type == TOKEN_REDIRECT_IN || current->type == TOKEN_APPEND_REDIRECTION || current->type == TOKEN_HERE_DOC) {
            cmd_node->redirects = current; // For simplicity, only handling last redirect. A list or array might be needed.
            current = current->next;
        } else {
            break;
        }
    }

    *tokens = current;
    return cmd_node;
}

command_node* parse_line(token *tokens) {
    command_node *head = NULL, *prev = NULL;

    while (tokens) {
        command_node *current = parse_command(&tokens);

        if (!head) {
            head = current;
            prev = head;
        } else {
            prev->next = current;
            prev = current;
        }

        if (tokens && tokens->type == TOKEN_PIPE) {
            tokens = tokens->next;  // Skip the pipe token and proceed to the next command.
        } else {
            break;
        }
    }

    return head;
}
