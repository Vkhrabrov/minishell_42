/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkhrabro <vkhrabro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 17:44:14 by vadimhrabro       #+#    #+#             */
/*   Updated: 2023/10/20 23:27:27 by vkhrabro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void reset_command_node(command_node* cmd) 
{
    cmd->command = NULL;
    cmd->args = NULL;
    cmd->redirect_in = NULL;
    cmd->redirect_out = NULL;
    cmd->here_doc_content = NULL;
    cmd->var_expansion = NULL;
    cmd->env_variable = NULL;
}

void free_command_node(command_node* node) 
{
    while (node) {
        command_node* next_node = node->next;

        // Free command
        if (node->command) {
            free(node->command->content);
            free(node->command);
        }

        // Free arguments
        token* arg = node->args;
        while (arg) {
            token* next_arg = arg->next;
            free(arg->content);
            free(arg);
            arg = next_arg;
        }

        // Free variable expansions
        if (node->var_expansion) {
            free(node->var_expansion->content);
            free(node->var_expansion);
        }

        // Free environment variables
        if (node->env_variable) {
            free(node->env_variable->content);
            free(node->env_variable);
        }

        // Free input redirection
        if (node->redirect_in) {
            free(node->redirect_in->content);
            free(node->redirect_in);
        }

        // Free here_doc_content if present
        if (node->here_doc_content) {
            free(node->here_doc_content);
        }

        // Free output redirection
        if (node->redirect_out) {
            free(node->redirect_out->content);
            free(node->redirect_out);
        }

        free(node);
        node = next_node;
    }
}

void print_command_node(command_node* node) 
{
    while (node) 
    {
        printf("Command Node:\n");

        // Print command
        if (node->command) 
            printf("  Command: %s\n", node->command->content);

        // Print arguments
        token *arg = node->args;
        while (arg) 
        {
            printf("  Argument: %s\n", arg->content);
            arg = arg->next;
        }

        // Print variable expansions
        if (node->var_expansion) 
        {
            printf("  Variable Expansion: %s\n", node->var_expansion->content);
        }

        // Print environment variables
        if (node->env_variable) 
        {
            printf("  Environment Variable: %s\n", node->env_variable->content);
        }

        // Print input redirection
        if (node->redirect_in) 
        {
            printf("  Redirection In: %s ", node->redirect_in->content);
            if (node->redirect_in->next)
                printf(" %s", node->redirect_in->next->content);
            printf("\n");
        }

        // Print here_doc_content if present
        if (node->here_doc_content)
        {
            printf("  HERE_DOC Content: %s\n", node->here_doc_content);
        }

        // Print output redirection
        if (node->redirect_out) 
        {
            printf("  Redirection Out: %s ", node->redirect_out->content);
            if (node->redirect_out->next)
                printf(" %s", node->redirect_out->next->content);
            printf("\n");
        }

        printf("--------------------------\n");
        node = node->next;
    }
}



char *append_line(char *existing_content, char *new_line) 
{
    if (!existing_content) 
        return ft_strdup(new_line);
    char *joined = ft_strjoin(existing_content, "\n");
    char *result = ft_strjoin(joined, new_line);
    free(joined);
    return result;
}

char *read_heredoc_content(const char *delimiter) 
{
    char *input_line;
    char *all_content;

    all_content = NULL;
    while (1) 
    {
        input_line = readline("> ");
        if (ft_strncmp(input_line, (char *)delimiter, ft_strlen(delimiter)) == 0) 
        {
            free(input_line);
            break;
        }
        char *temp_content = append_line(all_content, input_line);
        free(all_content);
        all_content = temp_content;
        free(input_line);
    }
    return all_content;
}

command_node* parse_command(token **tokens) 
{
    if (!tokens || !(*tokens)) return NULL;
    
    command_node *cmd_node = malloc(sizeof(command_node));
    reset_command_node(cmd_node);

     
    cmd_node->command = NULL;
    cmd_node->args = NULL;
    cmd_node->redirect_in = NULL;
    cmd_node->redirect_out = NULL;
    cmd_node->next = NULL;
    cmd_node->here_doc_content = NULL;
    cmd_node->var_expansion = NULL;
    cmd_node->env_variable = NULL;

    token *current = *tokens;
    token *last_arg = NULL;

    while (current && current->type != TOKEN_PIPE) 
    {
        if (current->type == TOKEN_COMMAND) 
        {
            if (!cmd_node->command)
                cmd_node->command = current;
            current = current->next;
        } 
        else if (current->type == TOKEN_ARGUMENT) 
        {
            if (!cmd_node->args)
                cmd_node->args = current;
            last_arg = current;
            current = current->next;
        }
        else if (current->type == TOKEN_VARIABLE_EXPANSION) 
        {
            cmd_node->var_expansion = current;
            last_arg = current;
            current = current->next;
        }
        else if (current->type == TOKEN_ENV_VARIABLE) 
        {
            cmd_node->env_variable = current;
            last_arg = current;
            current = current->next;
        } 
        else if (current->type == TOKEN_HERE_DOC) 
        {
            if (current->next == NULL || current->next->type != TOKEN_HEREDOC_DELIM)
            {
                printf("minishell: syntax error near unexpected token `newline'\n");
                return NULL;
            }
            cmd_node->redirect_in = current;
            current = current->next;
            if (current->type == TOKEN_HEREDOC_DELIM) 
            {
                char *delimiter = current->content;
                cmd_node->here_doc_content = read_heredoc_content(delimiter);
                current = current->next;
            }
        }
        else if (current->type == TOKEN_REDIRECT_IN || current->type == TOKEN_APPEND_REDIRECTION) 
        {
            cmd_node->redirect_in = current;
            current = current->next;
            if (!current || current->type != TOKEN_ARGUMENT) 
                return NULL;
            current = current->next;
        }
        else if (current->type == TOKEN_REDIRECT_OUT) 
        {
            cmd_node->redirect_out = current;
            current = current->next;
            if (!current || current->type != TOKEN_ARGUMENT) 
            {
                printf("Error: Expected file after redirection symbol.\n");
                return NULL;
            }
            current = current->next;
        }

        else
            break;
    }
    if (last_arg)
        last_arg->next = NULL;
    *tokens = current;
    return cmd_node;
}

command_node* parse_line(token *tokens) 
{
    command_node *head = NULL, *prev = NULL;

    while (tokens) 
    {
        
        command_node *current = parse_command(&tokens);

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
        if (tokens && tokens->type == TOKEN_PIPE)
            tokens = tokens->next;
        else if (tokens && (tokens->type == TOKEN_REDIRECT_IN
            || tokens->type == TOKEN_APPEND_REDIRECTION)) 
        {
            tokens = tokens->next;   
            if (!tokens || (tokens->type != TOKEN_COMMAND && tokens->type != TOKEN_ARGUMENT)) 
            {
                printf("Error: Expected file after redirection symbol.\n");
                return NULL;
            }
            current->redirect_in = tokens;
            current->redirect_out = tokens;
            tokens = tokens->next;
        } 
        else
            break;
    }
    return head;
}