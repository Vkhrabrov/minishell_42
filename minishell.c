/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkhrabro <vkhrabro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 18:55:56 by vkhrabro          #+#    #+#             */
/*   Updated: 2023/10/27 23:18:06 by vkhrabro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

const char* token_type_to_string(tokentype type) 
{
    switch (type) 
    {
        case T_CMD: return "COMMAND";
        case T_ARG: return "ARGUMENT";
        case T_PIPE: return "PIPE";
        case T_REDIR_IN: return "REDIRECT_IN";
        case T_REDIR_OUT: return "REDIRECT_OUT";
        case T_HEREDOC: return "HERE_DOC";
        case T_APP_REDIR: return "APPEND_REDIRECTION";
        case T_VAR_EXP: return "VARIABLE_EXPANSION";
        case T_EXIT_STATUS: return "EXIT_STATUS";
        case T_HEREDOC_DELIM: return "HEREDOC_DELIM";
        case T_ENV_VAR: return "ENV_VARIABLE";
        default: return "UNKNOWN";
    }
}

void print_token(token *t) 
{
    printf("Token content: '%s', Token type: %s\n", t->content,
        token_type_to_string(t->type));
}

int if_redir(char c)
{
    if (c == '>' || c == '<')
        return (1);
    else
        return (0); 
}

int main(int argc, char **argv, char **envp)
{

    token *tokens = NULL;
    command_node *head = NULL;
    t_env_lst  *env_lst = NULL;
    (void)argc;
    (void)argv;

    // env_lst = malloc(sizeof(t_env_lst));
    
    // t_env_init(env_lst);
    save_env_list(&env_lst, envp);
    // print_env_lst(&env_lst);
    disable_control_chars_echo();   //  Disable echoing of control characters (^C, ^\)
    init_signals();
    while (1) 
    {
        char *input = readline("minishell> ");
        if (!input) break; 
        tokens = tokenization(input);
        head = parse_line(tokens);
        // print_command_node(head);
        expand_environment_variables(head, &env_lst);
        print_command_node(head);
        process_command_list(head, env_lst); 
        // free_command_node(head);
        // reset_command_node(head);
        add_history(input);
        free(input);
    }
    // restore_terminal_settings();    //  Restore terminal settings before exiting
    return (0);
}