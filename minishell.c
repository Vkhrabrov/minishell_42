/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 18:55:56 by vkhrabro          #+#    #+#             */
/*   Updated: 2023/12/20 23:16:39 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_shlvl(t_env_lst **env_lst)
{
	int	shlvl;

	shlvl = ft_atoi(get_env_var_val(*env_lst, "SHLVL"));
	if (shlvl < 0)
		shlvl = 0;
	else if (shlvl != 0 && shlvl % 1000 == 0)
	{
		build_error_msg("warning: shell level (", ft_itoa(shlvl + 1), \
			") too high, resetting to 1", false);
		shlvl = 1;
	}
	else if (shlvl != 0)
		shlvl++;
	update_env_var_value(*env_lst, "SHLVL", ft_itoa(shlvl));
}

void	bash_exit_emulate(t_env_lst *env_lst)
{
	free_env_list(env_lst);
	ft_putstr_fd("\033[A", 1);
	ft_putstr_fd("\0\33[2K", 1);
	printf("%s", "minishell> exit\n");
}

void	handle_command_without_args(struct command_node *head, \
		t_env_lst *env_lst)
{
	restore_terminal_settings();
	set_noninteractive_signals();
	process_command_list(head, env_lst);
	disable_control_chars_echo();
	set_interactive_signals();
}

void	run_minishell_loop(t_env_lst *env_lst, struct command_node *head)
{
	char			*input;
	struct token	*tokens;
	int				num_tokens;

	while (1)
	{
		input = readline("minishell> ");
		if (!input)
			break ;
		tokens = tokenization(input);
		num_tokens = ft_list_size(tokens);
		head = parse_line(tokens);
		expand_environment_variables(head, &env_lst);
		if (head)
		{
			if (arg_alone(head, num_tokens) == 1)
				handle_command_without_args(head, env_lst);
			else
				process_command_list(head, env_lst);
		}
		else
			continue ;
		add_history(input);
		free(input);
	}
}

int	main(int argc, char **argv, char **envp)
{
	struct command_node	*head;
	t_env_lst			*env_lst;
	int					random_fd;

	(void)argc;
	(void)argv;
	head = NULL;
	env_lst = NULL;
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
	run_minishell_loop(env_lst, head);
	bash_exit_emulate(env_lst);
	restore_terminal_settings();
	return (g_exitstatus);
}
