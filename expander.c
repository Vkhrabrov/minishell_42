/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkhrabro <vkhrabro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 21:35:50 by vkhrabro          #+#    #+#             */
/*   Updated: 2023/12/13 23:20:25 by vkhrabro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

token *create_new_token(char *content, tokentype type) {
    token *new_token = (token *)malloc(sizeof(token));
    if (!new_token) {
        // Handle malloc failure
        return NULL;
    }

    new_token->content = ft_strdup(content);
    if (!new_token->content) {
        // Handle strdup failure
        free(new_token);
        return NULL;
    }

    new_token->type = type;
    new_token->next = NULL;
    return new_token;
}

void replace_env_with_token(command_node *current_command, char *env_value) {
    tokentype type = current_command->command ? T_ARG : T_CMD;
    token *new_token = create_new_token(env_value, type);
    if (!new_token) {
        // Handle create_new_token failure
        return;
    }

    if (type == T_ARG) {
        new_token->next = current_command->args;
        current_command->args = new_token;
    } else {
        new_token->next = current_command->command;
        current_command->command = new_token;
    }

    if (current_command->var_expansion) {
        free(current_command->var_expansion->content);
        free(current_command->var_expansion);
        current_command->var_expansion = NULL;
    }

    if (current_command->env_variable) {
        free(current_command->env_variable->content);
        free(current_command->env_variable);
        current_command->env_variable = NULL;
    }
}

void expand_environment_variables(command_node *cmds, t_env_lst **env_lst) {
    command_node *current_command = cmds;
    while (current_command) {
        if (current_command->env_variable) {
            t_env_lst *current_env = *env_lst;
            bool found = false; // Add a flag to track if the variable is found

            while (current_env) {
                if (ft_strncmp(current_env->var_name, current_command->env_variable->content, ft_strlen(current_command->env_variable->content)) == 0) {
                    if (current_env->var_value && *current_env->var_value) { // Check if the variable has a value
                        replace_env_with_token(current_command, current_env->var_value);
                        found = true;
                        break;
                    } else {
                        // Variable found but has no value
                        g_exitstatus = 0;
                        return;
                    }
                }
                current_env = current_env->next;
            }

            if (!found) {
                // Variable not found in the environment list
                g_exitstatus = 0;
                return;
            }
        }

        if (current_command->ex_status) {
            char *exit_status_str = ft_itoa(g_exitstatus);
            if (!exit_status_str) {
                // Handle ft_itoa failure
                current_command = current_command->next;
                continue;
            }
            replace_env_with_token(current_command, exit_status_str);
            free(exit_status_str);
        }
        current_command = current_command->next;
    }
}
