/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkhrabro <vkhrabro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 20:55:03 by vkhrabro          #+#    #+#             */
/*   Updated: 2024/02/01 20:54:15 by vkhrabro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	process_token_conditions(struct command_node *cmd,
	struct token **current, struct token **prev, char *env_value)
{
	if (!cmd->new_token && (*current)->type != T_EX_ST)
		cmd->new_token = create_new_tokens(prev, *current, env_value, cmd);
	if ((*current)->type != T_ENV_VAR && (*current)->type != T_VAR_EXP)
		cmd->new_token = create_new_tokens(prev, *current, env_value, cmd);
	if (*prev)
		(*prev)->next = cmd->new_token;
	else
		cmd->command = cmd->new_token;
	cmd->new_token->next = (*current)->next;
	if ((*current)->next)
		(*current)->next->prev = cmd->new_token;
	free_token(*current);
	cmd->i--;
	*prev = cmd->new_token;
	if ((*current)->type == T_EX_ST)
		token_insert(cmd, cmd->new_token);
	if (cmd->new_token != NULL)
		*current = cmd->new_token->next;
	else
		*current = NULL;
}

void	insert_new_token(struct command_node *command, char *env_value)
{
	struct token	*current;
	struct token	*prev;

	current = command->command;
	prev = NULL;
	command->i = 2;
	command->new_token = NULL;
	while (current)
	{
		if (current->type == T_ENV_VAR
			|| current->type == T_VAR_EXP || current->type == T_EX_ST)
		{
			process_token_conditions(command, &current, &prev, env_value);
			if (command->i == 0)
			{
				token_insert(command, prev);
				break ;
			}
		}
		else
		{
			prev = current;
			current = current->next;
		}
	}
}

void	cleanup_old_data(struct command_node *current_command)
{
	if (current_command->var_expansion)
	{
		free(current_command->var_expansion->content);
		free(current_command->var_expansion);
		current_command->var_expansion = NULL;
	}
	if (current_command->env_variable)
	{
		free(current_command->env_variable->content);
		free(current_command->env_variable);
		current_command->env_variable = NULL;
	}
}

void	process_env_var_token(struct token *token,
			t_env_lst **env_lst, struct command_node *current_command)
{
	t_env_lst	*current_env;

	current_env = *env_lst;
	while (current_env != NULL)
	{
		if (ft_strncmp(current_env->var_name, token->content,
				ft_strlen(token->content)) == 0)
		{
			if (current_env->var_value && *current_env->var_value)
				insert_new_token(current_command, current_env->var_value);
			else
			{
				g_exitstatus = 0;
				return ;
			}
			break ;
		}
		current_env = current_env->next;
	}
}

void	handle_env_var(struct command_node *current_command,
			t_env_lst **env_lst)
{
	struct token	*token;

	token = current_command->command;
	while (token)
	{
		if (token->type == T_ENV_VAR)
			process_env_var_token(token, env_lst, current_command);
		token = token->next;
	}
}
