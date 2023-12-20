/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkhrabro <vkhrabro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 18:55:56 by vkhrabro          #+#    #+#             */
/*   Updated: 2023/12/19 23:29:56 by vkhrabro         ###   ########.fr       */
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

int if_redir(char c)
{
    if (c == '>' || c == '<')
        return (1);
    else
        return (0); 
}
extern int rl_eof_found;

void	update_shlvl(t_env_lst **env_lst)
{
	int	shlvl; 
	
	shlvl = ft_atoi(get_env_var_val(*env_lst, "SHLVL"));
	if (shlvl < 0)
		shlvl = 0;
	else if (shlvl != 0 && shlvl % 1000 == 0)
	{
		build_error_msg("warning: shell level (", ft_itoa(shlvl + 1), ") too high, resetting to 1", false);
		shlvl = 1;
	}
	else if (shlvl != 0)
		shlvl++;
	update_env_var_value(*env_lst, "SHLVL", ft_itoa(shlvl));
}

void	bash_exit_emulate(t_env_lst *env_lst)
{
	if (rl_eof_found)
    {
		free_env_list(env_lst);
        ft_putstr_fd("\033[A", 1);
        ft_putstr_fd("\0\33[2K", 1);
        printf("%s", "minishell> exit\n");
    }	
}

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
    
    save_env_list(&env_lst, envp);
	update_shlvl(&env_lst);
    disable_control_chars_echo();
	set_interactive_signals();

    while (1) 
    {
        char *input = readline("minishell> ");
        if (!input) 
			break;
        tokens = tokenization(input);
        int num_tokens = ft_list_size(tokens);
        head = parse_line(tokens);
        expand_environment_variables(head, &env_lst);
        // exit(1);
        // print_command_node(head);
        if(head)
        {
        // print_command_node(head);
if (num_tokens == 1 || 
    (head && head->command && head->command->content && 
        ((ft_strncmp(head->command->content, "cat", 3) == 0 && ft_strlen(head->command->content) == 3) || 
         (ft_strncmp(head->command->content, "wc", 2) == 0 && ft_strlen(head->command->content) == 2))))           
         {
                restore_terminal_settings();
                set_noninteractive_signals();
                process_command_list(head, env_lst);
                disable_control_chars_echo();
                set_interactive_signals();
            }
            else 
                    process_command_list(head, env_lst);	
        }
        else
            continue;	
        // free_command_node(head);
        // reset_command_node(head);
        add_history(input);
        free(input);
    }
	bash_exit_emulate(env_lst);
    restore_terminal_settings();
    return (g_exitstatus);
}
