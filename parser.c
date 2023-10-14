/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkhrabro <vkhrabro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 17:44:14 by vadimhrabro       #+#    #+#             */
/*   Updated: 2023/10/13 20:23:07 by vkhrabro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void print_command_node(command_node* node) {
    while (node) {
        printf("Command Node:\n");
        if (node->command) {
            printf("  Command: %s\n", node->command->content);
        }
        token *arg = node->args;
        while (arg) {
            printf("  Argument: %s\n", arg->content);
            arg = arg->next;
        }
        if (node->redirects) {
            printf("  Redirection: %s\n", node->redirects->content);
            // Assuming the file token is the next token after the redirection token
            if (node->redirects->next) {
                printf("  File: %s\n", node->redirects->next->content);
            }
        }
        printf("--------------------------\n");
        node = node->next;
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
    token *last_arg = NULL;

    while (current && current->type != TOKEN_PIPE) {
        if (current->type == TOKEN_COMMAND) {
            if (!cmd_node->command) {
                cmd_node->command = current;  // Assuming only one token for command.
            }
            current = current->next;
        } else if (current->type == TOKEN_ARGUMENT) {
            if (!cmd_node->args) {
                cmd_node->args = current;  // Capture the first argument token.
            }
            last_arg = current;
            current = current->next;
        } else if (current->type == TOKEN_REDIRECT_OUT || current->type == TOKEN_REDIRECT_IN || current->type == TOKEN_APPEND_REDIRECTION || current->type == TOKEN_HERE_DOC) {
            cmd_node->redirects = current; // Capture redirection
            current = current->next;
            if (current->type == TOKEN_ARGUMENT) {
                current = current->next;  // Skip the file token (don't add it as an argument)
            }
        } else {
            break;
        }
    }

    // Disconnect the token list at the last argument token
    if (last_arg) {
        last_arg->next = NULL;
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
