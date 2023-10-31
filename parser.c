/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkhrabro <vkhrabro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 17:44:14 by vadimhrabro       #+#    #+#             */
/*   Updated: 2023/10/30 19:36:41 by vkhrabro         ###   ########.fr       */
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
        if (ft_strncmp(input_line, (char *)delimiter, ft_strlen(delimiter)) == 0)
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
    
    command_node *cmd_node = malloc(sizeof(command_node));
    token *current = *tokens;
    token *last_arg = NULL;

    while (current && current->type != T_PIPE) 
    {
        if (current->type == T_CMD) 
        {
            if (!cmd_node->command)
                cmd_node->command = current;
            current = current->next;
        } 
        else if (current->type == T_ARG) 
        {
            if (!cmd_node->args)
                cmd_node->args = current;
            last_arg = current;
            current = current->next;
        }
        // if (current->type == T_EXIT_STATUS) 
        // {
        //     if (!cmd_node->command)
        //         cmd_node->command = current;
        //     current = current->next;
        // } 
        else if (current->type == T_VAR_EXP) 
        {
            cmd_node->var_expansion = current;
            last_arg = current;
            current = current->next;
        }
        else if (current->type == T_ENV_VAR) 
        {
            cmd_node->env_variable = current;
            last_arg = current;
            current = current->next;
        } 
        else if (current->type == T_HEREDOC) 
        {
            if (current->next == NULL || current->next->type != T_HEREDOC_DELIM)
            {
                printf("minishell: syntax error near unexpected token `newline'\n");
                return NULL;
            }
            cmd_node->redirect_in = current;
            current = current->next;
            if (current->type == T_HEREDOC_DELIM) 
            {
                char *delimiter = current->content;
                cmd_node->here_doc_content = read_heredoc_content(delimiter);
                current = current->next;
            }
        }
        else if (current->type == T_REDIR_IN || current->type == T_APP_REDIR) 
        {
            cmd_node->redirect_in = current; // Save the redirection symbol
            current = current->next;
            if (!current || current->type != T_ARG) 
                return NULL;
            cmd_node->redirect_in_filename = ft_strdup(current->content); // Save the filename
            current = current->next;
        }
        else if (current->type == T_REDIR_OUT) 
        {
            cmd_node->redirect_out = current; // Save the redirection symbol
            current = current->next;
            if (!current || current->type != T_ARG) 
            {
                printf("Error: Expected file after redirection symbol.\n");
                return NULL;
            }
            cmd_node->redirect_out_filename = ft_strdup(current->content); // Save the filename
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