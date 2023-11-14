/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 17:44:14 by vadimhrabro       #+#    #+#             */
/*   Updated: 2023/11/14 20:36:00 by ccarrace         ###   ########.fr       */
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
        if (node->here_doc_content)
            printf("  HERE_DOC Content: %s\n", node->here_doc_content);
        if (node->redirect_in) 
            printf("  Redirection In: %s\n  Filename: %s\n", node->redirect_in->content, node->redirect_in_filename);
        if (node->redirect_out) 
            printf("  Redirection Out: %s\n  Filename: %s\n", node->redirect_out->content, node->redirect_out_filename);
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
		/* CARLOS: heredoc exited if the strings passed contained the delimiter (e.g 'eofhola') */
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
    token *temp_last_token = NULL;

    while (current && current->type != T_PIPE) 
    {
        if (!current) break;

        if (current->type == T_CMD) 
        {
            cmd_node->command = current;
        }
        else if (current->type == T_ARG) 
        {
            if (!cmd_node->args)
                cmd_node->args = current;
            temp_last_token = current;
        }
        else if (current->type == T_VAR_EXP) 
        {
            cmd_node->var_expansion = current;
            temp_last_token = current;
        }
        else if (current->type == T_ENV_VAR) 
        {
            cmd_node->env_variable = current;
            temp_last_token = current;
        }
        else if (current->type == T_HEREDOC) 
        {
            if (current->next && current->next->type == T_HEREDOC_DELIM) 
            {
                cmd_node->redirect_in = current;
                char *delimiter = current->next->content;
                cmd_node->here_doc_content = read_heredoc_content(delimiter);
                current = current->next;
            } 
            else 
            {
                printf("minishell: syntax error near unexpected token `newline'\n");
                return NULL;
            }
        }
        else if (current->type == T_REDIR_IN || current->type == T_APP_REDIR) 
        {
            cmd_node->redirect_in = current;
            if (current->next && current->next->type == T_ARG)
            {
                cmd_node->redirect_in_filename = ft_strdup(current->next->content);
                current = current->next;
            }
        }
        else if (current->type == T_REDIR_OUT) 
        {
            cmd_node->redirect_out = current;
            if (current->next && current->next->type == T_ARG)
            {
                cmd_node->redirect_out_filename = ft_strdup(current->next->content);
                current = current->next;
            }
        }

        current = current->next;
    }
    if (temp_last_token)
        temp_last_token->next = NULL;
    *tokens = current;
    return cmd_node;
}



command_node* parse_line(token *tokens) 
{
    command_node    *head;
    command_node    *prev;

    head = NULL; 
    prev = NULL;
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
        if (tokens && tokens->type == T_PIPE)
            tokens = tokens->next;
        else if (tokens && (tokens->type == T_REDIR_IN
            || tokens->type == T_APP_REDIR)) 
        {
            tokens = tokens->next;   
            if (!tokens || (tokens->type != T_CMD && tokens->type != T_ARG)) 
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
    return (head);
}