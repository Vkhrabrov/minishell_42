/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkhrabro <vkhrabro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 18:55:56 by vkhrabro          #+#    #+#             */
/*   Updated: 2023/12/03 20:26:56 by vkhrabro         ###   ########.fr       */
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
extern int rl_eof_found;

int main(int argc, char **argv, char **envp)
{
    int random_fd;
    token *tokens = NULL;
    command_node *head = NULL;
    t_env_lst  *env_lst = NULL;
    (void)argc;
    (void)argv;
    g_exitstatus = 0;
    random_fd = open("fd_test", O_TRUNC | O_CREAT | O_RDWR, 0644);
    if (random_fd == -1)
    {
        perror("");
        exit(EXIT_FAILURE);
    }
    close(random_fd);
    // env_lst = malloc(sizeof(t_env_lst));
    
    // t_env_init(env_lst);
    save_env_list(&env_lst, envp);
    // print_env_lst(&env_lst);
    disable_control_chars_echo();   //  Disable echoing of control characters (^C, ^\)
    init_signals();

	// enum shell_mode mode;
	// if (isatty(STDIN_FILENO))
	// {
	// 	mode = INTERACTIVE;
	// 	printf("out: INTERACTIVE MODE\n");
	// }
	// else
	// {
	// 	mode = NON_INTERACTIVE;
	// 	printf("out: NON INTERACTIVE MODE\n");
	// }
	
    while (1) 
    {
        char *input = readline("minishell> ");
        if (!input) 
			break;
		// if (mode == INTERACTIVE)
		// 	printf("in: INTERACTIVE MODE\n");
		// else
		// 	printf("in: NON INTERACTIVE MODE\n");
        tokens = tokenization(input);
        head = parse_line(tokens);
        // print_command_node(head);
        expand_environment_variables(head, &env_lst);
        // print_command_node(head);
        process_command_list(head, env_lst);
        
        // free_command_node(head);
        // reset_command_node(head);
        add_history(input);
        free(input);
    }
	// free_env_list(env_lst);
	if (rl_eof_found)
    {
		free_env_list(env_lst);
        ft_putstr_fd("\033[A", 1);
        ft_putstr_fd("\0\33[2K", 1);
        printf("%s", "minishell> exit\n");
    }

    // restore_terminal_settings();    //  Restore terminal settings before exiting
    // printf("%d\n", g_exitstatus);
    return (g_exitstatus);
}
