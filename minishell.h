/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkhrabro <vkhrabro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 22:58:44 by vkhrabro          #+#    #+#             */
/*   Updated: 2023/12/20 20:59:12 by vkhrabro         ###   ########.fr       */
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

enum shell_mode
{
    INTERACTIVE,
    NON_INTERACTIVE
};

typedef enum 
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
}             tokentype;

typedef struct token token;

struct token
{
    char       *content;
    tokentype   type;
    token      *next;
    token      *prev;
};

typedef struct redirection {
    token *type;        // The token for the redirection type ('>', '>>', or '<')
    char *filename;     // The filename for the redirection
    struct redirection *next;  // Pointer to the next redirection
} redirection;

int g_exitstatus;

typedef struct command_node command_node;

typedef struct s_exec_context
{
    char **paths;
    char *full_cmd_path;
    pid_t pid;
    char **final_args;
    char **final_env;
    int status;
} t_exec_context;

struct command_node 
{
    token           *command;
    token           *args;
    redirection     *redirects;
    token           *redirect_in;
    token           *redirect_out;
    token           *redirect_append;
    token           *redirections;
    token           *var_expansion;
    token           *env_variable;
    token           *ex_status;
    char            *redirect_in_filename;
    char            *redirect_out_filename;
    char            *here_doc_content;
    int             exit_status;

    command_node    *next;
};

typedef struct tokenizer_state 
{
    int i;
    tokentype prev_type;
    int expect_command;
    int expect_filename_after_redir;
    int last_was_redir_arg;
    token *tokens;
} tokenizer_state;

typedef struct  s_env_lst
{
    char				*var_name;
    char				*var_value;
	struct s_env_lst	*next;
}
t_env_lst;


//	Vadim's intrincate stuff
command_node*   parse_line(token *tokens);
void            free_command_node(command_node* node); 
void            reset_command_node(command_node* cmd);
token           *creat_token(const char* content, tokentype type);
token           *get_last_token(token *head);
void            add_to_list(token **head, token *new_token);
// char            *substring(const char *input, int start, int end);
char            *substring(char *input_string, int start, int end);
// char            *lex_quoted_string(char *input_string, int *i);
char            *lex_quoted_string(char *input_string, int *i, char end_char);
token           *tokenization(char *input);
void            handle_commands_and_args(char *input, tokenizer_state *state);
void            handle_special_tokens(char c, char *input, tokenizer_state *state);
void            handle_redirection(char c, char *input, tokenizer_state *state);
void            handle_quoted_string(char c, char *input, tokenizer_state *state);
int             if_redir(char c);
void            handle_commands_and_args(char *input, tokenizer_state *state);
tokentype       c_a_part_2(tokenizer_state *state, tokentype current_type);
tokenizer_state init_tokenizer_state(void);
size_t          find_char_index(const char *str, char target);
void            add_env_var_to_list(t_env_lst **head, const char *envp_line);
void            save_env_list(t_env_lst **env_lst, char **envp);
void            t_env_init(t_env_lst  *env_lst);
void            expand_environment_variables(command_node *command, t_env_lst **env_lst); 
int 			process_command_list(command_node *head, t_env_lst *env_lst); 
const char      *token_type_to_string(tokentype type);
void            handle_outfile(command_node *cmd_node);
char            **convert_command_node_args_to_array(command_node *cmd_node);
int             count_env_list_elements(t_env_lst *env_lst);
char            **convert_env_list_to_array(t_env_lst *env_lst);
char            *get_env_value(t_env_lst *env_lst, const char *key);
char            **get_paths_from_env(t_env_lst *env_lst);
char            *concatenate_paths_libft(const char *path, const char *cmd);
char            *search_command_in_paths(const char *cmd, char **paths);
char            *check_command_accessibility(const char *cmd);
int             execute_command_node(command_node *cmd_node, t_env_lst *env_lst);
// handling redirections (executor 5)
void            handle_here_doc(command_node *cmd_node);
void            handle_infile(command_node *cmd_node);
void            handle_outfile(command_node *cmd_node);
void            execution_checks(command_node *cmd_node, t_exec_context *exec_ctx);
char            *find_command_path(const char *cmd, char **paths);
// related to pipex processes  (executor 4 and 6)
void            child_process(command_node *cmd_node, t_env_lst *env_lst, int in_fd, int out_fd);
void            handle_child_process(command_node *current, t_env_lst *env_lst, int in_fd, int end[]);
void            fd_pipex_change(int *in_fd, int *end, command_node **current);
int             setup_and_pipe_loop(command_node *head, command_node **current, int *in_fd, t_env_lst *env_lst);
int             pipex(command_node *head, t_env_lst *env_lst);
void            parent_process_actions(int *status, pid_t pid);
int             final_cleanup_and_exit_status();
void            child_process_fd_handler(int original_stdout, int original_stdin, command_node *cmd_node);
// expander related
token           *create_new_token(char *content, tokentype type);
void            insert_new_token(command_node *current_command, char *env_value);
void            cleanup_old_data(command_node *current_command);
void            replace_env_with_token(command_node *current_command, char *env_value);
void            handle_environment_variable(command_node *current_command, t_env_lst **env_lst);

//	parser
command_node	*parse_command(token **tokens);
char			*read_heredoc_content(const char *delimiter);
void			add_redirection(command_node *cmd_node, token *redir_token, char *filename);
void            process_other_tokens(token *current, command_node *cmd_node);

//	Debug
void            print_tokens(const token *t);
void            print_command_node(command_node* head);

//	Clean
void			free_env_list(t_env_lst *env_lst);

//	Signals
void            set_interactive_signals(void);
void            set_noninteractive_signals(void);
// void            rl_replace_line (const char *text, int clear_undo);
void            disable_control_chars_echo();
void            restore_terminal_settings();
// void 			enable_control_chars_echo(void) ;

//	Builtins
int				execute_builtin(char *cmd_name, command_node *cmd_node, t_env_lst *env_lst);
int				echo_builtin(t_env_lst *env_lst, token *args_lst);
int				pwd_builtin(t_env_lst *env_lst);
int				env_builtin(t_env_lst *env_lst, token *args_lst);
int				cd_builtin(t_env_lst *env_lst, token *args_lst);
int				export_builtin(t_env_lst *env_lst, token *args_lst);
int 			unset_builtin(t_env_lst **env_lst, token *args_lst);
int				exit_builtin(token *args_lst);
void			free_args_list(token *args_lst);

//	Cd specific utilities
int				is_path_null(t_env_lst *env_lst, char *path);
int				are_hyphens_valid(t_env_lst *env_lst, char *path);
bool 			is_arg_properly_quoted(char *arg);
char			*get_curr_work_dir();

//	Echo specific utilities
bool			is_valid_numeric(const char *str);
void			remove_leading_zeros(char *str);
void			remove_trailing_spaces(char *str);

//	Export specific utilities
void			print_export_list(t_env_lst **env_lst);
bool			is_var_name_valid(char *arg, int equal_sign_position);
t_env_lst 		*insert_sorted(t_env_lst **head, t_env_lst *new_node);
void			add_new_var(t_env_lst **head, char *arg, size_t equal_sign_position);

//	Builtins general utilities
int				find_max_len(char *str1, char *str2);
long			ft_atol(const char *str);
char			*ft_ltoa(long n);
int				ft_list_size(token *args_lst);
char			*get_env_var_val(t_env_lst *env_lst, char *str);
int				update_env_var_value(t_env_lst *env_lst, char *sought_name, char *new_value);
int             is_builtin(command_node *cmd_node);
int             builtin_process(command_node *cmd_node, t_env_lst *env_lst);

//	Builtins errors
int				build_error_msg(char *command_name, char *arg, char *err_description, bool quoted);

#endif 