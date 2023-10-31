/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 23:30:07 by vkhrabro          #+#    #+#             */
/*   Updated: 2023/10/31 20:07:17 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *concatenate_paths_libft(const char *path, const char *cmd) {
    char *slash = "/";
    char *temp1 = ft_strjoin(path, slash);
    if (!temp1) {
        perror("ft_strjoin");
        exit(EXIT_FAILURE);
    }
    char *result = ft_strjoin(temp1, cmd);
    if (!result) {
        perror("ft_strjoin");
        exit(EXIT_FAILURE);
    }
    free(temp1);
    return result;
}

char **convert_command_node_args_to_array(command_node *cmd_node) {
    int count = 0;
    token *arg_temp = cmd_node->args;

    // Count the number of arguments in the linked list
    while (arg_temp) {
        count++;
        arg_temp = arg_temp->next;
    }

    // Allocate space for arguments array + 1 for the command + 1 for NULL termination
    char **args_array = malloc(sizeof(char *) * (count + 2));
    if (!args_array) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    // First argument is the command itself
    args_array[0] = strdup(cmd_node->command->content);
    if (!args_array[0]) {
        perror("strdup");
        exit(EXIT_FAILURE);
    }

    // Fill the args_array with the arguments
    int i = 1;
    arg_temp = cmd_node->args;
    while (arg_temp) {
        args_array[i] = strdup(arg_temp->content);
        if (!args_array[i]) {
            perror("strdup");
            exit(EXIT_FAILURE);
        }
        i++;
        arg_temp = arg_temp->next;
    }
    args_array[i] = NULL; // NULL terminate the array

    return args_array;
}

char **convert_env_list_to_array(t_env_lst *env_lst) {
    int count = 0;
    t_env_lst *temp = env_lst;
    while (temp) {
        count++;
        temp = temp->next;
    }

    char **env_array = malloc(sizeof(char *) * (count + 1));
    if (!env_array) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    int i = 0;
    while (env_lst) {
        int length = ft_strlen(env_lst->var_name) + ft_strlen(env_lst->var_value) + 2;
        env_array[i] = malloc(length);
        if (!env_array[i]) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        // snprintf(env_array[i], length, "%s=%s", env_lst->var_name, env_lst->var_value);
        i++;
        env_lst = env_lst->next;
    }
    env_array[i] = NULL;

    return env_array;
}

char *get_env_value(t_env_lst *env_lst, const char *key) {
    while (env_lst) {
        if (ft_strncmp(env_lst->var_name, (char *)key, ft_strlen(env_lst->var_name)) == 0)
            return env_lst->var_value;
        env_lst = env_lst->next;
    }
    return NULL;
}

char **get_paths_from_env(t_env_lst *env_lst) {
    char *path_string = get_env_value(env_lst, "PATH");
    if (!path_string)
        return NULL;

    return ft_split(path_string, ':');  // Assuming you have ft_split
}

char *find_command_path(const char *cmd, char **paths) {
    char *full_cmd_path = NULL;
    if (cmd[0] == '/' || cmd[0] == '.' || ft_strchr(cmd, '/')) {
        if (access(cmd, X_OK) == 0) {
            full_cmd_path = ft_strdup(cmd);
            if (!full_cmd_path) {
                perror("ft_strdup");
                exit(EXIT_FAILURE);
            }
        }
    } else {
        for (int i = 0; paths && paths[i]; i++) {
            int length = ft_strlen(paths[i]) + ft_strlen(cmd) + 2; 
            char *temp_path = malloc(length);
            if (!temp_path) {
                perror("malloc");
                exit(EXIT_FAILURE);
            }
            temp_path = concatenate_paths_libft(paths[i], cmd);
            if (access(temp_path, X_OK) == 0) {
                full_cmd_path = temp_path;
                break;
            }
            free(temp_path);
        }
    }
    return full_cmd_path;
}

void handle_here_doc(command_node *cmd_node) {
    if (!cmd_node->here_doc_content) 
        return;

    int pipefd[2];
    if (pipe(pipefd) == -1) 
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    write(pipefd[1], cmd_node->here_doc_content, ft_strlen(cmd_node->here_doc_content));
    close(pipefd[1]);

    dup2(pipefd[0], STDIN_FILENO);
    close(pipefd[0]);
}

void execute_command_node(command_node *cmd_node, t_env_lst *env_lst) 
{
    char **paths = get_paths_from_env(env_lst);
    char *full_cmd_path = find_command_path(cmd_node->command->content, paths);
    pid_t pid = fork();
    if (!full_cmd_path) {
        perror(cmd_node->command->content);
        exit(127);
    }

    char **final_args = convert_command_node_args_to_array(cmd_node);
    char **final_env = convert_env_list_to_array(env_lst);

    if (pid == 0) 
    {  // This block will be executed by the child process
        if (cmd_node->here_doc_content) 
            handle_here_doc(cmd_node);
        
        execve(full_cmd_path, final_args, final_env);
        perror("execve");
        exit(1);
    } 
    else 
    {  // This block will be executed by the parent process
        int status;
        waitpid(pid, &status, 0);
    }
    free(full_cmd_path);
    for (int i = 0; final_args[i]; i++)
        free(final_args[i]);
    free(final_args);

    for (int i = 0; final_env[i]; i++) {
        free(final_env[i]);
    }
    free(final_env);
}


void process_command_list(command_node *head, t_env_lst *env_lst) {
    int node_count = 0;
    command_node *current = head;
    // List of built-in commands (add your built-ins to this list)
    char *builtins[] = {"cd", "echo", "env", "exit", "export", "pwd", "unset", NULL}; 

    while (current) {
        node_count++;
        current = current->next;
    }

    if (node_count > 1) {
        printf("Launching pipex...\n");
        return;
    }

    if (node_count == 1) {
        int is_builtin = 0;
        for (int i = 0; builtins[i]; i++) {
            if (ft_strncmp(head->command->content, builtins[i], ft_strlen(builtins[i])) == 0)
            {
                is_builtin = 1;
                break;
            }
        }
        if (is_builtin) {
            printf("Executing built-in command: %s\n", head->command->content);
			execute_builtin(head->command->content, head, *env_lst);
        } else {
            printf("Launching pipex execution process...\n");
        }
    }
}
