/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utility_functions.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkhrabro <vkhrabro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/21 21:43:29 by vkhrabro          #+#    #+#             */
/*   Updated: 2024/01/25 21:27:43 by vkhrabro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

struct token	*creat_token(const char *content, enum tokentype type)
{
	struct token	*new_token;

	new_token = (struct token *)malloc(sizeof(struct token));
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

struct token	*get_last_token(struct token *head)
{
	if (!head)
		return (NULL);
	while (head->next)
		head = head->next;
	return (head);
}

/*	extract_quoted_segment()
 *
 *  (89) start after the initial quote
 * 	(91) find the end of the quoted string
 * 	(98) extract and concatenate this segment
 *	(104) move past the closing quote
 */
char	*extract_quoted_segment(char *input_string, char *line, int *i, \
		char end_char)
{
	int		start;
	char	*temp;

	start = (*i) + 1;
	(*i)++;
	while (input_string[*i] && input_string[*i] != end_char)
		(*i)++;
	if (!input_string[*i])
	{
		free(line);
		exit (1);
	}
	temp = substring(input_string, start, *i - 1);
	if (temp)
	{
		ft_strlcat(line, temp, ft_strlen(line) + ft_strlen(temp) + 1);
		free(temp);
	}
	(*i)++;
	return (line);
}

/*	extract_non_quoted_segment()
 *
 *	Handles non-quoted segment immediately following a quote
 */
char	*extract_non_quoted_segment(char *input_string, char *line, int *i)
{
	char	*temp;
	int		start;

	start = *i;
	while (input_string[*i] && input_string[*i] != ' ' \
		&& input_string[*i] != '"' && input_string[*i] != '\'')
		(*i)++;
	temp = substring(input_string, start, *i - 1);
	if (temp)
	{
		ft_strlcat(line, temp, ft_strlen(line) + ft_strlen(temp) + 1);
		free(temp);
	}
	return (line);
}

/*	lex_quoted_string()
*
*	Extracts and concatenates segments from input skipping quotes
*	(see extract_quoted_segment() and extract_non_quoted_segment() above)
*
*	(151) Stops if a space is encountered
*	(159) Breaks the loop if next character is not a quote
*/
char	*lex_quoted_string(char *input_string, int *i, char end_char)
{
	char	*line;

	line = malloc(ft_strlen(input_string) + 1);
	if (!line)
		return (NULL);
	line[0] = '\0';
	while (input_string[*i])
	{
		if (input_string[*i] == end_char)
			extract_quoted_segment(input_string, line, i, end_char);
		if (input_string[*i] != ' ')
		{
			extract_non_quoted_segment(input_string, line, i);
			if (input_string[*i] == ' ')
				break ;
		}
		if (input_string[*i] == '"' || input_string[*i] == '\'')
			end_char = input_string[*i];
		else
			break ;
	}
	return (line);
}
