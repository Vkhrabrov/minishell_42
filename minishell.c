/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 18:55:56 by vkhrabro          #+#    #+#             */
/*   Updated: 2023/10/08 23:40:27 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/*
void create_empty_list(void)
{
    token.value = NULL;
    token.type = NULL;
}

Token* create_token(const char* content, TokenType type) 
{
    // Allocate memory for the Token structure
    Token* new_token = (Token*)malloc(sizeof(Token));
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
    return (new_token);
}


char *substring(input_string, start, i)
{
    char *substring;
    int k;
    int t;

    k = i - start + 1;
    t = 0;
    substring = malloc((sizeof)(char) * k + 1);
    while (t < k)
        substring[t++] = input_string[start++];
    substring[t] = '\0';
    return (substring);
}

void tokenization(char *input)
{
    char* input_string;
    int i;
    List* tokens;

    input_string = input;
    token = create_empty_list();

    // Now, let's iterate over the input string, character by character
    i = 0;
    while (i < strlen(input_string)) 
    {
        char c = input_string[i];

        // If the character is a space, we simply skip to the next character.
        if (c == ' ')
            continue;

        // If the character is a quote, we should keep reading characters 
        // until we find the closing quote.
        if (c == '"') 
        {
            int start = i + 1; // Start just after the quote
            i++; // Move to next character    
            while (input_string[i] != '"' && i < strlen(input_string)) 
                i++;
            // Now extract the substring between start and i
            char* quoted_string = substring(input_string, start, i);
            add_to_list(tokens, create_token(quoted_string, TOKEN_ARGUMENT));
        }
        // Handling other token types (like pipes or redirects) can be done in a similar manner.
        // For example, if we encounter a '|', we can add a TOKEN_PIPE token to our list.
        else if (c == '|')
            add_to_list(tokens, create_token("|", TOKEN_PIPE));
        // Here, you would handle other special characters similarly...

        // ...

        // If the character doesn't fall into any special category,
        // we treat it as a regular command or argument.
        else 
        {
            int start = i;
            
            // Read until we encounter a space or another special character.
            // This is where you'd extend checks for other special characters too.
            while (input_string[i] != ' ' && input_string[i] != '|' && i < strlen(input_string))
                i++;

            char* command_or_arg = substring(input_string, start, i - 1);
            add_to_list(tokens, create_token(command_or_arg, // decide if it's a command or argument ));
            i--; // Because the outer for-loop will increment i again
        }
        i++;
    }
}
*/

int main(int argc, char **argv, char **envp)
{
    int i;

    i = 0;
    (void)argc;
    (void)argv;
    (void)envp;

    disable_control_chars_echo();   //  Disable echoing of control characters (^C, ^\)
    init_signals();
    while (1) 
    {
        char *input = readline("minishell> ");
        if (!input) break; 
        // tokenization(input);
        add_history(input);
        free(input);

        // Check if the next character is EOF
        int next_char = getchar();
        if (next_char == EOF)
        {
          // Print the "exit" message and close the program
          printf("exit\n");
          break;
        }
    }
    restore_terminal_settings();    //  Restore terminal settings before exiting
    return (0);
}