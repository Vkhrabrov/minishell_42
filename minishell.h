/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 22:58:44 by vkhrabro          #+#    #+#             */
/*   Updated: 2023/12/20 22:07:41 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <termios.h>    // Contains 'tcsetattr()' and 'tcgetattr()'
# include <limits.h>     // PATH_SIZE
# include <unistd.h>
# include "libft/libft.h" 
# include <unistd.h>
# include <string.h>
# include <fcntl.h>
# include <stdbool.h>
# include <errno.h>

// Error messages definition
# define MS_TOOMANYARG	"too many arguments"
# define MS_NOTSET 		" not set"
# define MS_NOTNUMARG	": numeric argument required"
# define MS_INVALDOPT	": invalid option"
# define MS_NOFILEDIR	": No such file or directory"
# define MS_NOTDIR		": Not a directory"
# define MS_ACCESFORB	": Permission denied"
# define MS_LONGNAME		": File name too long"
# define MS_BADID	": not a valid identifier"

typedef enum tokentype
{
	T_CMD,
	T_ARG,
	T_PIPE,
	T_NONE,
	T_REDIR_IN,
	T_REDIR_OUT,
	T_HEREDOC,
	T_APP_REDIR,
	T_VAR_EXP,
	T_BACKGROUND_EXEC,
	T_EXIT_STATUS,
	T_HEREDOC_DELIM,
	T_ENV_VAR,
}	t_tokentype;

typedef struct token
{
	char					*content;
	enum tokentype			type;
	struct token			*next;
	struct token			*prev;
}	t_token;

typedef struct redirection
{
	struct token		*type;
	char				*filename;
	struct redirection	*next;
}	t_redirection;

int	g_exitstatus;

typedef struct command_node
{
	struct token			*command;
	struct token			*args;
	struct redirection		*redirects;
	struct token			*redirect_in;
	struct token			*redirect_out;
	struct token			*redirect_append;
	struct token			*redirections;
	struct token			*var_expansion;
	struct token			*env_variable;
	struct token			*ex_status;
	char					*redirect_in_filename;
	char					*redirect_out_filename;
	char					*here_doc_content;
	int						exit_status;
	struct command_node		*next;
}	t_command_node;

typedef struct tokenizer_state
{
	int				i;
	enum tokentype	prev_type;
	int				expect_command;
	int				expect_filename_after_redir;
	int				last_was_redir_arg;
	struct token	*tokens;
}	t_tokenizer_state;

typedef struct s_env_lst
{
	char				*var_name;
	char				*var_value;
	struct s_env_lst	*next;
}	t_env_lst;

//	Vadim's intrincate stuff
struct command_node		*parse_line(struct token *tokens);
void					free_command_node(struct command_node *node);
void					reset_command_node(struct command_node *cmd);
struct token			*creat_token(const char *content, enum tokentype type);
struct token			*get_last_token(struct token *head);
void					add_to_list(struct token **head, \
						struct token *new_token);
char					*substring(char *input_string, int start, int end);
char					*lex_quoted_string(char *input_string, int *i, \
						char end_char);
struct token			*tokenization(char *input);
void					handle_commands_and_args(char *input, \
						struct tokenizer_state *state);
void					handle_special_tokens(char c, char *input, \
						struct tokenizer_state *state);
void					handle_redirection(char c, char *input, \
						struct tokenizer_state *state);
void					handle_quoted_string(char c, char *input, \
						struct tokenizer_state *state);
int						if_redir(char c);
void					handle_commands_and_args(char *input, \
						struct tokenizer_state *state);
enum tokentype			c_a_part_2(struct tokenizer_state *state, \
						enum tokentype current_type);
struct tokenizer_state	init_tokenizer_state(void);
size_t					find_char_index(const char *str, char target);
void					add_env_var_to_list(t_env_lst **head, \
						const char *envp_line);
void					save_env_list(t_env_lst **env_lst, char **envp);
void					t_env_init(t_env_lst *env_lst);
void					expand_environment_variables( \
						struct command_node *command, t_env_lst **env_lst);
int						process_command_list(struct command_node *head, \
						t_env_lst *env_lst);
const char				*token_type_to_string(enum tokentype type);
void					handle_outfile(struct command_node *cmd_node);

//	minishell1
int 					arg_alone_or_first_is_cat_or_wc \
						(struct command_node *head, int num_tokens);

//	parser
struct command_node		*parse_command(struct token **tokens);
char					*read_heredoc_content(const char *delimiter);
void					add_redirection(struct command_node *cmd_node, \
						struct token *redir_token, char *filename);
void					process_other_tokens(struct token *current, \
						struct command_node *cmd_node);
int						handle_pipe_tokens(struct token **tokens, \
						struct command_node **current);

//	Debug
void					print_tokens(const struct token *t);
void					print_command_node(struct command_node *head);

//	Clean
void					free_env_list(t_env_lst *env_lst);

//	Signals
void					set_interactive_signals(void);
void					set_noninteractive_signals(void);
void					disable_control_chars_echo(void);
void					restore_terminal_settings(void);

//	Builtins
int						execute_builtin(char *cmd_name, \
						struct command_node *cmd_node, t_env_lst *env_lst);
int						echo_builtin(t_env_lst *env_lst, \
						struct token *args_lst);
int						pwd_builtin(t_env_lst *env_lst);
int						env_builtin(t_env_lst *env_lst, \
						struct token *args_lst);
int						cd_builtin(t_env_lst *env_lst, struct token *args_lst);
int						export_builtin(t_env_lst *env_lst, \
						struct token *args_lst);
int						unset_builtin(t_env_lst **env_lst, \
						struct token *args_lst);
int						exit_builtin(struct token *args_lst);
void					free_args_list(struct token *args_lst);

//	Cd specific utilities
int						is_path_null(t_env_lst *env_lst, char *path);
int						are_hyphens_valid(t_env_lst *env_lst, char *path);
bool					is_arg_properly_quoted(char *arg);
char					*get_curr_work_dir(void);

//	Echo specific utilities
bool					is_valid_numeric(const char *str);
void					remove_leading_zeros(char *str);
void					remove_trailing_spaces(char *str);

//	Export specific utilities
void					print_export_list(t_env_lst **env_lst);
bool					is_var_name_valid(char *arg, int equal_sign_position);
t_env_lst				*insert_sorted(t_env_lst **head, t_env_lst *new_node);
void					add_new_var(t_env_lst **head, char *arg, \
						size_t equal_sign_position);

//	Builtins general utilities
int						find_max_len(char *str1, char *str2);
long					ft_atol(const char *str);
char					*ft_ltoa(long n);
int						ft_list_size(struct token *args_lst);
char					*get_env_var_val(t_env_lst *env_lst, char *str);
int						update_env_var_value(t_env_lst *env_lst, \
						char *sought_name, char *new_value);

//	Builtins errors
int						build_error_msg(char *command_name, char *arg, \
						char *err_description, bool quoted);

#endif 