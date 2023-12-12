/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utility_functions.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkhrabro <vkhrabro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/21 21:43:29 by vkhrabro          #+#    #+#             */
/*   Updated: 2023/12/12 21:07:06 by vkhrabro         ###   ########.fr       */
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

char	*lex_quoted_string(char *input_string, int *i, char end_char)
{
	int	start;

	start = (*i) + 1;
	(*i)++;
	while (input_string[*i] != end_char && *i < (int)ft_strlen(input_string))
		(*i)++;
	return (substring(input_string, start, *i - 1));
}
