/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkhrabro <vkhrabro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 22:58:44 by vkhrabro          #+#    #+#             */
/*   Updated: 2023/12/03 20:27:28 by vkhrabro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <termios.h>    // Contains 'tcsetattr()' and 'tcgetattr()'
#include <limits.h>     // PATH_SIZE
#include <unistd.h>
#include "libft/libft.h" 
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdbool.h>
#include <errno.h>

// Error messages definition
#define MS_TOOMANYARG	"too many arguments"
#define MS_NOTSET 		" not set"
#define MS_NOTNUMARG	": numeric argument required"
#define MS_INVALDOPT	": invalid option"
#define MS_NOFILEDIR	": No such file or directory"
#define MS_NOTDIR		": Not a directory"
#define MS_ACCESFORB	": Permission denied"
#define MS_LONGNAME		": File name too long"
#define MS_BADID	": not a valid identifier"

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
};

int g_exitstatus;

typedef struct command_node command_node;

struct command_node 
{
    token           *command;
    token           *args;
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
void            print_command_node(command_node* head);
void            free_command_node(command_node* node); 
void            reset_command_node(command_node* cmd);
token           *creat_token(const char* content, tokentype type);
token           *get_last_token(token *head);
void            add_to_list(token **head, token *new_token);
char            *substring(char *input_string, int start, int end);
char            *lex_quoted_string(char *input_string, int *i, char end_char);
void            print_token(token *t);
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
void			free_env_list(t_env_lst *env_lst);
void            print_env_list(t_env_lst **env_lst);
void            t_env_init(t_env_lst  *env_lst);
void            expand_environment_variables(command_node *command, t_env_lst **env_lst); 
int 			process_command_list(command_node *head, t_env_lst *env_lst); 
const char      *token_type_to_string(tokentype type);

//	Signals
void            init_signals(void);
void            rl_replace_line (const char *text, int clear_undo);
void            disable_control_chars_echo();
void            restore_terminal_settings();

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

//	Builtins general utilities
int				find_max_len(char *str1, char *str2);
long			ft_atol(const char *str);
char			*ft_ltoa(long n);
int				ft_list_size(token *args_lst);
char			*get_env_var_value(t_env_lst *env_lst, char *str);
int				update_env_var_value(t_env_lst *env_lst, char *sought_name, char *new_value); 

//	Builtins errors
int				build_error_msg(char *command_name, char *arg, char *err_description, bool quoted);

#endif 