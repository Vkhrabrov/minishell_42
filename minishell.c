/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vadimhrabrov <vadimhrabrov@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 18:55:56 by vkhrabro          #+#    #+#             */
/*   Updated: 2023/10/08 21:00:55 by vadimhrabro      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void create_empty_list(void)
// {
//     token.value = NULL;
//     token.type = NULL;
// }

const char* token_type_to_string(tokentype type) 
{
    switch (type) 
    {
        case TOKEN_COMMAND: return "COMMAND";
        case TOKEN_ARGUMENT: return "ARGUMENT";
        case TOKEN_PIPE: return "PIPE";
        case TOKEN_REDIRECT_IN: return "REDIRECT_IN";
        case TOKEN_REDIRECT_OUT: return "REDIRECT_OUT";
        // ... add other cases as needed
        default: return "UNKNOWN";
    }
}

void print_token(token *t) 
{
    printf("Token content: '%s', Token type: %s\n", t->content, token_type_to_string(t->type));
}

token   *create_token(const char* content, tokentype type) 
{
    // Allocate memory for the Token structure
    token   *new_token = (token*)malloc(sizeof(token));
    if (!new_token) 
    {
        perror("Failed to allocate memory for token");
        exit(EXIT_FAILURE);
    }
    // Allocate memory for the content of the token and copy the given content into it
    new_token->content = ft_strdup(content);
    if (!new_token->content) {
        perror("Failed to allocate memory for token content");
        free(new_token);
        exit(EXIT_FAILURE);
    }
    // Set the type of the token
    new_token->type = type;
    new_token->next = NULL;
    return (new_token);
}

void add_to_list(token **head, token *new_token) 
{
    if (!*head) 
    {
        *head = new_token;
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

    // Now, let's iterate over the input string, character by character
    i = 0;
    while (i < (int)ft_strlen(input_string)) 
    {
        char c = input_string[i];

        // If the character is a space, we simply skip to the next character.
        if (c == ' ')
        {
            i++;
            continue;
        }

        // If the character is a quote, we should keep reading characters 
        // until we find the closing quote.
        if (c == '"') 
        {
            int start = i + 1; // Start just after the quote
            i++; // Move to next character    
            while (input_string[i] != '"' && i < (int)ft_strlen(input_string)) 
                i++;
            // Now extract the substring between start and i
            quoted_string = substring(input_string, start, i);
            add_to_list(&tokens, create_token(quoted_string, TOKEN_ARGUMENT));
        }
        // Handling other token types (like pipes or redirects) can be done in a similar manner.
        // For example, if we encounter a '|', we can add a TOKEN_PIPE token to our list.
        else if (c == '|')
            add_to_list(&tokens, create_token("|", TOKEN_PIPE));
        // Here, you would handle other special characters similarly...

        // ...

        // If the character doesn't fall into any special category,
        // we treat it as a regular command or argument.
        else 
        {
            int start = i;
            
            // Read until we encounter a space or another special character.
            // This is where you'd extend checks for other special characters too.
            while (input_string[i] != ' ' && input_string[i] != '|' && i < (int)ft_strlen(input_string))
                i++;

            command_or_arg = substring(input_string, start, i - 1);
            add_to_list(&tokens, create_token(command_or_arg, TOKEN_COMMAND));
            i--; // Because the outer for-loop will increment i again
        }
        i++;
    }
}


int main(int argc, char **argv, char **envp)
{
    (void)argc;
    (void)argv;
    (void)envp;
    // init_signals();
    while (1) 
    {
        char *input = readline("minishell> ");
        if (!input) break; 
        tokenization(input);
        add_history(input);
        free(input);
    }
    return (0);
}