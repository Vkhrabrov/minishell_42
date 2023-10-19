/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkhrabro <vkhrabro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 18:55:56 by vkhrabro          #+#    #+#             */
/*   Updated: 2023/10/20 00:15:27 by vkhrabro         ###   ########.fr       */
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
        case TOKEN_VARIABLE_EXPANSION: return "VARIABLE_EXPANSION";
        case TOKEN_EXPAND_TO_EXIT: return "EXPAND_TO_EXIT";
        case TOKEN_HEREDOC_DELIM: return "HEREDOC_DELIM";
        case TOKEN_ENV_VARIABLE: return "ENV_VARIABLE";
        default: return "UNKNOWN";
    }
}

token *get_last_token(token *head) 
{
    if (!head) return NULL;
    while (head->next) {
        head = head->next;
    }
    return head;
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

int if_redirection(char c)
{
    if (c == '>' || c == '<')
        return (1);
    else
        return (0); 
}

char *lex_quoted_string(char *input_string, int *i, char end_char) 
{
    int start = (*i) + 1;
    (*i)++;
    while (input_string[*i] != end_char && *i < (int)ft_strlen(input_string)) 
        (*i)++;
    return substring(input_string, start, *i - 1);
}

token *tokenization(token *tokens, char *input) {
    int i = 0;
    
    tokentype prev_type = TOKEN_NONE;
    int expect_command = 1;
    int expect_filename_after_redir = 0;

    while (i < (int)ft_strlen(input)) {
        char c = input[i];

        // Skip whitespace
        if (c == ' ' || c == '\t') {
            i++;
            continue;
        }

        // Handle quoted strings
        if (c == '"' || c == '\'') {
            char end_char = c;
            char *arg = lex_quoted_string(input, &i, end_char);

            tokentype current_type = TOKEN_ARGUMENT;
            if (prev_type == TOKEN_PIPE || prev_type == TOKEN_NONE || prev_type == TOKEN_REDIRECT_IN || prev_type == TOKEN_REDIRECT_OUT)
                current_type = TOKEN_COMMAND;
            else if (prev_type == TOKEN_VARIABLE_EXPANSION)
                current_type = TOKEN_ENV_VARIABLE;
            add_to_list(&tokens, create_token(arg, current_type));
            free(arg);

            prev_type = current_type;
        }
        else if (c == '|') 
        {
            add_to_list(&tokens, create_token("|", TOKEN_PIPE));
            prev_type = TOKEN_PIPE;
            expect_command = 1;
        } 
        else if (c == '$')
        {
            add_to_list(&tokens, create_token("$", TOKEN_VARIABLE_EXPANSION));
            prev_type = TOKEN_VARIABLE_EXPANSION;
        }
        else if (if_redirection(c)) 
        {
            char next_char = input[i + 1];
            if ((c == '>' && next_char == '>') || (c == '<' && next_char == '<')) {
                char redir[3];
                sprintf(redir, "%c%c", c, next_char);
                add_to_list(&tokens, create_token(redir, (c == '>') ? TOKEN_APPEND_REDIRECTION : TOKEN_HERE_DOC));
                i++;
            } 
            else {
                add_to_list(&tokens, create_token(&c, (c == '>') ? TOKEN_REDIRECT_OUT : TOKEN_REDIRECT_IN));
            }
            prev_type = (c == '>') ? TOKEN_REDIRECT_OUT : TOKEN_REDIRECT_IN;
            expect_filename_after_redir = 1;
        } 
        else {
            int start = i;
            tokentype current_type = TOKEN_COMMAND;
            while (i < (int)ft_strlen(input) && !if_redirection(input[i]) && input[i] != ' ' && input[i] != '|') {
                i++;
            }
            char *command_or_arg = substring(input, start, i - 1);
             if (expect_command) {
                current_type = TOKEN_COMMAND;
                expect_command = 0; // Reset it
            } else {
                current_type = TOKEN_ARGUMENT;
            }
            if (expect_filename_after_redir) 
            {
                current_type = TOKEN_ARGUMENT;
                expect_filename_after_redir = 0;  // Reset the flag
                expect_command = 1;
            }
            if (prev_type == TOKEN_VARIABLE_EXPANSION)
                current_type = TOKEN_ENV_VARIABLE;
            add_to_list(&tokens, create_token(command_or_arg, current_type));
            free(command_or_arg); // Don't forget to free the memory!
            prev_type = current_type;
            i--;
        }
        i++;
    }
    return (tokens);
}

int main(int argc, char **argv, char **envp)
{

    token *tokens = NULL;
    command_node *head = NULL;
    (void)argc;
    (void)argv;
    (void)envp;

    disable_control_chars_echo();   //  Disable echoing of control characters (^C, ^\)
    init_signals();
    while (1) 
    {
        char *input = readline("minishell> ");
        if (!input) break; 
        tokens = tokenization(tokens, input);
        head = parse_line(tokens);
        print_command_node(head);
        // free_command_node(head);
        // reset_command_node(head);
        add_history(input);
        free(input);
    }
    // restore_terminal_settings();    //  Restore terminal settings before exiting
    return (0);
}