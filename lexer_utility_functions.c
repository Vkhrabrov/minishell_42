/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utility_functions.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkhrabro <vkhrabro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/21 21:43:29 by vkhrabro          #+#    #+#             */
/*   Updated: 2023/12/13 23:09:13 by vkhrabro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

token	*creat_token(const char *content, tokentype type)
{
	token	*new_token;

	new_token = (token *)malloc(sizeof(token));
	if (!new_token)
	{
		perror("Failed to allocate memory for token");
		exit(EXIT_FAILURE);
	}
	new_token->content = ft_strdup(content);
	if (!new_token->content)
	{
		perror("Failed to allocate memory for token content");
		free(new_token);
		exit(EXIT_FAILURE);
	}
	new_token->type = type;
	new_token->next = NULL;
	new_token->prev = NULL;
	return (new_token);
}

token	*get_last_token(token *head)
{
	if (!head)
		return (NULL);
	while (head->next)
		head = head->next;
	return (head);
}

void add_to_list(token **head, token *new_token) 
{
    token *last_token;

    if (!*head) {
        *head = new_token;
    } else {
        last_token = get_last_token(*head);
        last_token->next = new_token;
        new_token->prev = last_token; // Set the prev pointer of the new token
    }
}

char	*substring(char *input_string, int start, int end)
{
	char	*substring;
	int		k;
	int		t;

	k = end - start + 1;
	t = 0;
	substring = malloc(sizeof(char) * k + 1);
	while (t < k)
		substring[t++] = input_string[start++];
	substring[t] = '\0';
	return (substring);
}

char *lex_quoted_string(char *input_string, int *i, char end_char) {
    int start;
    char *temp, *line;

    line = malloc(ft_strlen(input_string) + 1);
    if (!line) return NULL;
    line[0] = '\0';

    while (input_string[*i]) {
        if (input_string[*i] == end_char) {
            start = (*i) + 1;  // Start after the initial quote
            (*i)++;

            // Find the end of this quoted string
            while (input_string[*i] && input_string[*i] != end_char) {
                (*i)++;
            }
            if (!input_string[*i]) {
                free(line);
                return NULL;
            }

            // Extract and concatenate this segment
            temp = substring(input_string, start, *i - 1);
            if (temp) {
                strcat(line, temp);
                free(temp);
            }

            (*i)++;  // Move past the closing quote
        } 
		if (input_string[*i] != ' ') {
            // Handle non-quoted segment immediately following a quote
            start = *i;
            while (input_string[*i] && input_string[*i] != ' ' && input_string[*i] != '"' && input_string[*i] != '\'') {
                (*i)++;
            }
            temp = substring(input_string, start, *i - 1);
            if (temp) {
                strcat(line, temp);
                free(temp);
            }
            if (input_string[*i] == ' ') break; // Stop if a space is encountered
        }

        // Update end_char for the next quoted segment, if any
        if (input_string[*i] == '"' || input_string[*i] == '\'') {
            end_char = input_string[*i];
        } else {
            break;  // Break if the next character is not a quote
        }
    }

    return line;
}





