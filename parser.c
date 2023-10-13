/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkhrabro <vkhrabro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 17:44:14 by vadimhrabro       #+#    #+#             */
/*   Updated: 2023/10/13 19:43:14 by vkhrabro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void print_command_nodes(command_node* head) {
    command_node* current = head;

    while (current) {
        printf("Command Node:\n");

        if (current->command) {
            printf("  Command: %s\n", current->command->content);
        } else {
            printf("  Command: NULL\n");
        }

        if (current->args) {
            printf("  Argument: %s\n", current->args->content);
        } else {
            printf("  Argument: NULL\n");
        }

        if (current->redirects) {
            printf("  Redirection: %s", current->redirects->content);
            if (current->redirects->next && (current->redirects->next->type == TOKEN_ARGUMENT || current->redirects->next->type == TOKEN_COMMAND)) {
                printf(" %s\n", current->redirects->next->content);
            } else {
                printf("\n");
            }
        } else {
            printf("  Redirection: NULL\n");
        }

        printf("--------------------------\n");
        current = current->next;
    }
}


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

command_node* parse_line(token *tokens) 
{
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
        }
        // Handle redirections (This part is pseudocode and requires a proper structure for command_node)
        else if (tokens && (tokens->type == TOKEN_REDIRECT_OUT || tokens->type == TOKEN_REDIRECT_IN || tokens->type == TOKEN_APPEND_REDIRECTION || tokens->type == TOKEN_HERE_DOC)) {
            // token* redirection = tokens;
            tokens = tokens->next;
            
            if (!tokens || (tokens->type != TOKEN_COMMAND && tokens->type != TOKEN_ARGUMENT)) {
                printf("Error: Expected file after redirection symbol.\n");
                return NULL;
            }

            current->redirects = tokens;  // Set redirection target (file)
            tokens = tokens->next;
        } else {
            break;
        }
    }

    return head;
}
