/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 18:55:56 by vkhrabro          #+#    #+#             */
/*   Updated: 2023/10/10 00:20:13 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

const char* token_type_to_string(tokentype type) 
{
    switch (type) 
    {
        case TOKEN_COMMAND: return "COMMAND";
        case TOKEN_ARGUMENT: return "ARGUMENT";
        case TOKEN_PIPE: return "PIPE";
        case TOKEN_REDIRECT_IN: return "REDIRECT_IN";
        case TOKEN_REDIRECT_OUT: return "REDIRECT_OUT";
        case TOKEN_HERE_DOC: return "HERE_DOC";
        case TOKEN_APPEND_REDIRECTION: return "APPEND_REDIRECTION";
        case TOKEN_BACKGROUND_EXEC: return "BACKGROUND_EXEC";
        case TOKEN_EXPAND_TO_EXIT: return "EXPAND_TO_EXIT";
        default: return "UNKNOWN";
    }
}

void print_token(token *t) 
{
    printf("Token content: '%s', Token type: %s\n", t->content, token_type_to_string(t->type));
}

token   *create_token(const char* content, tokentype type) 
{
    token   *new_token = (token*)malloc(sizeof(token));
    if (!new_token) 
    {
        perror("Failed to allocate memory for token");
        exit(EXIT_FAILURE);
    }
    new_token->content = ft_strdup(content);
    if (!new_token->content) {
        perror("Failed to allocate memory for token content");
        free(new_token);
        exit(EXIT_FAILURE);
    }
    new_token->type = type;
    new_token->next = NULL;
    return (new_token);
}

void add_to_list(token **head, token *new_token) 
{
    if (!*head) 
    {
        *head = new_token;
        print_token(new_token);
        return;
    }
    token* temp = *head;
    while (temp->next)
        temp = temp->next;
    temp->next = new_token;
    print_token(new_token);
}

char *substring(char *input_string, int start, int end)
{
    char *substring;
    int k;
    int t;

    k = end - start + 1;
    t = 0;
    substring = malloc(sizeof(char) * k + 1);
    while (t < k)
        substring[t++] = input_string[start++];
    substring[t] = '\0';
    return (substring);
}

void tokenization(char *input)
{
    char    *input_string;
    int     i;
    token   *tokens;
    char    *quoted_string;
    char    *command_or_arg;

    input_string = input;
    tokens = NULL;
    i = 0;
    while (i < (int)ft_strlen(input_string)) 
    {
        char c = input_string[i];
        if (c == ' ')
        {
            i++;
            continue;
        }
        if (c == '"') 
        {
            int start = i + 1;
            i++;
            while (input_string[i] != '"' && i < (int)ft_strlen(input_string)) 
                i++;
            quoted_string = substring(input_string, start, i - 1);
            add_to_list(&tokens, create_token(quoted_string, TOKEN_ARGUMENT));
        }
        else if (c == '\'') 
        {
            int start = i + 1;
            i++;
            while (input_string[i] != '\'' && i < (int)ft_strlen(input_string)) 
                i++;
            quoted_string = substring(input_string, start, i - 1);
            add_to_list(&tokens, create_token(quoted_string, TOKEN_ARGUMENT));
        }
        else if (c == '|')
            add_to_list(&tokens, create_token("|", TOKEN_PIPE));
        else if ((c == '>') && input_string[i + 1] == '>')
        {
            add_to_list(&tokens, create_token(">>", TOKEN_APPEND_REDIRECTION));
            i++;
        }
        else if ((c == '<') && input_string[i + 1] == '<')
        {
            add_to_list(&tokens, create_token("<<", TOKEN_HERE_DOC));
            i++;
        }
        else if ((c == '>') && input_string[i + 1] != '>')
            add_to_list(&tokens, create_token(">", TOKEN_REDIRECT_OUT));
        else if (c == '<' && input_string[i + 1] != '<')
            add_to_list(&tokens, create_token("<", TOKEN_REDIRECT_IN));
        else if ((c == '>') && input_string[i + 1] == '>')
            add_to_list(&tokens, create_token(">>", TOKEN_APPEND_REDIRECTION));
        else if ((c == '$') && input_string[i + 1] != '?')
            add_to_list(&tokens, create_token("$", TOKEN_BACKGROUND_EXEC));
        else if ((c == '$') && input_string[i + 1] == '?')
        {
            add_to_list(&tokens, create_token("$?", TOKEN_EXPAND_TO_EXIT));
            i++;
        }
        else 
        {
            int start = i;
            while (input_string[i] != ' ' && input_string[i] != '|' && i < (int)ft_strlen(input_string))
                i++;

            command_or_arg = substring(input_string, start, i - 1);
            add_to_list(&tokens, create_token(command_or_arg, TOKEN_COMMAND));
            i--; 
        }
        i++;
    }
}

int main(int argc, char **argv, char **envp)
{
    (void)argc;
    (void)argv;
    (void)envp;

    disable_control_chars_echo();   //  Disable echoing of control characters (^C, ^\)
    init_signals();
    while (1) 
    {
        char *input = readline("minishell> ");
        if (!input) break; 
        tokenization(input);
        add_history(input);
        free(input);
    }
    // restore_terminal_settings();    //  Restore terminal settings before exiting
    return (0);
}