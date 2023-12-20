/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 21:35:50 by vkhrabro          #+#    #+#             */
/*   Updated: 2023/12/20 18:38:27 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

struct token *create_new_token(char *content, enum tokentype type) {
    struct token *new_token = (struct token *)malloc(sizeof(struct token));
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

void replace_env_with_token(struct command_node *current_command, char *env_value) {
    enum tokentype type = current_command->command ? T_ARG : T_CMD;
    struct token *new_token = create_new_token(env_value, type);
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

void expand_environment_variables(struct command_node *cmds, t_env_lst **env_lst) {
    struct command_node *current_command = cmds;
    char *dollar = NULL;
    char *start = NULL;
    char *new_env_var = NULL;
    t_env_lst *current_env = *env_lst;
    bool found = false;
    int i = 0;
    while (current_command) 
    {
        if (current_command->env_variable) 
        {
            while (current_env) 
            {
                if (ft_strncmp(current_env->var_name, current_command->env_variable->content, ft_strlen(current_command->env_variable->content)) == 0) 
                {
                    if (current_env->var_value && *current_env->var_value) 
                    { // Check if the variable has a value
                        replace_env_with_token(current_command, current_env->var_value);
                        found = true;
                        break;
                    } 
                    else 
                    {
                        // Variable found but has no value
                        g_exitstatus = 0;
                        return;
                    }
                }
                current_env = current_env->next;
            }

                if (!found) 
                {
                    // Variable not found in the environment list
                    g_exitstatus = 0;
                    return;
                }
        }
        
        if (current_command->args && (dollar = ft_strchr(current_command->args->content, '$')) != NULL &&
    ft_strchr(dollar, '|') != NULL)
{
    char *new_arg = ft_strdup(""); // Initialize with an empty string
    start = dollar;
    i = 0;
    while (*dollar != '|' && *dollar != '\0') // Check for end of string
    {
        dollar++;
        i++;
    }
    new_env_var = malloc(sizeof(char) * (i + 1));
    ft_strlcpy(new_env_var, start + 1, i);
    new_arg = ft_strjoin(new_arg, "|");

    while (current_env) 
    {
        if (ft_strncmp(current_env->var_name, new_env_var, find_max_len(current_env->var_name, new_env_var)) == 0) 
        {
            if (current_env->var_value && *current_env->var_value) 
                new_arg = ft_strjoin(new_arg, current_env->var_value);
         
            
        }
        current_env = current_env->next;
    }
    new_arg = ft_strjoin(new_arg, "|");
    ft_strlcpy(current_command->args->content, new_arg, ft_strlen(new_arg) + 1);
    free(new_env_var);
    free(new_arg);
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
