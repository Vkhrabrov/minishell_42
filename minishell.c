/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 18:55:56 by vkhrabro          #+#    #+#             */
/*   Updated: 2024/02/01 00:03:23 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_shlvl(t_env_lst **env_lst)
{
	int		shlvl;
	char	*shlvl_str;

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
	shlvl_str = ft_itoa(shlvl);
	update_env_var_value(*env_lst, "SHLVL", shlvl_str);
	free (shlvl_str);
}

void	handle_command_without_args(struct command_node *head, \
		t_env_lst *env_lst, struct token *tokens)
{
	restore_terminal_settings();
	set_noninteractive_signals();
	process_command_list(head, env_lst, tokens);
	disable_control_chars_echo();
	set_interactive_signals();
}

void	execute_command(struct command_node *head, t_env_lst *env_lst, \
	struct token *tokens, int num_tokens)
{
	if (arg_alone(head, num_tokens) == 1)
		handle_command_without_args(head, env_lst, tokens);
	else
		process_command_list(head, env_lst, tokens);
}

void	run_minishell_loop(t_env_lst *env_lst, struct command_node *head)
{
	char			*input;
	char			*input_copy;
	struct token	*tokens;
	int				num_tokens;

	while (1)
	{
		input = readline("minishell> ");
		if (!input)
			break ;
		input_copy = ft_strdup(input);
		tokens = tokenization(input);
		num_tokens = ft_list_size(tokens);
		head = parse_line(tokens);
		expand_environment_variables(head, &env_lst, input_copy);
		if (head)
			execute_command(head, env_lst, tokens, num_tokens);
		else
			continue ;
		add_history(input);
		free(input);
		if (input_copy)
			free(input_copy);
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
