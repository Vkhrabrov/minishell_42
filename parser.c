/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 17:44:14 by vadimhrabro       #+#    #+#             */
/*   Updated: 2023/12/10 13:55:55 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void print_command_node(command_node* node) 
{
    while (node) 
    {
        printf("Command Node:\n");
        if (node->command) 
            printf("  Command: %s\n", node->command->content);
        token *arg = node->args;
        while (arg) 
        {
            printf("  Argument: %s\n", arg->content);
            arg = arg->next;
        }
        if (node->var_expansion) 
            printf("  Variable Expansion: %s\n", node->var_expansion->content);
        if (node->env_variable) 
            printf("  Environment Variable: %s\n", node->env_variable->content);
        if (node->ex_status)
            printf("  Exit status: %s\n", node->ex_status->content);

        // Print redirections
        redirection *redir = node->redirects;
        while (redir) 
        {
            printf("  Redirection: %s\n  Filename: %s\n", redir->type->content, redir->filename);
            redir = redir->next;
        }

        if (node->here_doc_content)
            printf("  HERE_DOC Content: %s\n", node->here_doc_content);

        printf("--------------------------\n");
        node = node->next;
    }
}


char    *append_line(char *existing_content, char *new_line) 
{
    char    *joined;
    char    *result;
    if (!existing_content) 
        return (ft_strdup(new_line));
    joined = ft_strjoin(existing_content, "\n");
    result = ft_strjoin(joined, new_line);
    free(joined);
    return (result);
}

char    *read_heredoc_content(const char *delimiter) 
{
    char    *input_line;
    char    *all_content;
    char    *temp_content;

    all_content = NULL;
    while (1) 
    {
        input_line = readline("> ");
        if (ft_strncmp(input_line, (char *)delimiter, find_max_len(input_line, (char *)delimiter)) == 0)
        {
            free(input_line);
            break;
        }
        temp_content = append_line(all_content, input_line);
        free(all_content);
        all_content = temp_content;
        free(input_line);
    }
    return (all_content);
}

void add_redirection(command_node *cmd_node, token *redir_token, char *filename) {
    redirection *new_redir = malloc(sizeof(redirection));
    if (!new_redir) {
        perror("Failed to allocate memory for redirection");
        exit(EXIT_FAILURE);
    }
    new_redir->type = redir_token;
    new_redir->filename = strdup(filename);
    new_redir->next = NULL;

    // Append the new redirection to the linked list in the command_node
    if (cmd_node->redirects == NULL) {
        cmd_node->redirects = new_redir;
    } else {
        redirection *last = cmd_node->redirects;
        while (last->next != NULL) {
            last = last->next;
        }
        last->next = new_redir;
    }
}

bool is_redirection_token(tokentype type) {
    return type == T_REDIR_IN || type == T_APP_REDIR || type == T_REDIR_OUT;
}

command_node* parse_command(token **tokens) 
{
    if (!tokens || !(*tokens)) return NULL;
    command_node *cmd_node = ft_calloc(1, sizeof(command_node));
    if (!cmd_node) 
    {
        perror("Memory allocation failed for command_node");
        exit(EXIT_FAILURE);
    }
    
    token *current = *tokens;
    token **last_arg = &(cmd_node->args); // Pointer to the last argument's 'next' field

    while (current && current->type != T_PIPE) 
    {
        if (current->type == T_CMD) 
        {
            if (!cmd_node->command)
                cmd_node->command = current;
        }
        else if (current->type == T_ARG) 
        {
            *last_arg = current; // Link the argument
            last_arg = &current->next; // Update the last argument pointer
        }
        else if (is_redirection_token(current->type))
        {
            if (current->next && current->next->type == T_ARG) 
            {
                add_redirection(cmd_node, current, current->next->content);
                *last_arg = NULL; // Terminate the arguments list before redirection
                current = current->next; // Skip the filename token
            } 
            else 
            {
                printf("Error: Expected filename after redirection symbol.\n");
                while (current)
                    current = current->next;
                *tokens = current; 
                return NULL;
            }
        }
        else if (current->type == T_VAR_EXP) 
        {
            cmd_node->var_expansion = current;
        }
        else if (current->type == T_EXIT_STATUS)
        {
            cmd_node->ex_status = current;
        }
        else if (current->type == T_ENV_VAR) 
        {
            cmd_node->env_variable = current;
        }
        // Handle other token types as necessary

        current = current->next;
    }
    *last_arg = NULL;
    *tokens = current;
    return cmd_node;
}




command_node* parse_line(token *tokens) 
{
    command_node *head = NULL;
    command_node *prev = NULL;

    while (tokens) 
    {
        command_node *current = parse_command(&tokens);

        // Print current token's details

        if (!head) 
        {
            head = current;
            prev = head;
        } 
        else 
        {
            prev->next = current;
            prev = current;
        }

        // Move past the pipe token, if present
        if (tokens && tokens->type == T_PIPE)
            tokens = tokens->next;
    }
    return head;
}

