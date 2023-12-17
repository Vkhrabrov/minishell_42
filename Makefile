# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/05 19:26:19 by vkhrabro          #+#    #+#              #
#    Updated: 2023/12/17 00:45:28 by ccarrace         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
HEADER = minishell.h
LIBFT = libft/
READLINE = readline/

SRC_F:= minishell lexer lexer2 lexer_utility_functions lexer_utility_functions_2 \
		parser parser2 env_list_creation expander executor signals signals_utils clean \
		builtins/cd_builtin builtins/cd_utils builtins/echo_builtin \
		builtins/exit_builtin builtins/exit_utils builtins/env_builtin \
		builtins/export_builtin builtins/export_utils builtins/pwd_builtin \
		builtins/unset_builtin builtins/builtins_errors builtins/builtins_execute \
		builtins/builtins_utils_1 builtins/builtins_utils_2 \
		debug

FLAGS = -Wall -Wextra -Werror -g -MMD -I$(READLINE)/include 
RM = rm -f
SRC = $(addsuffix .c, $(SRC_F))
OBJS = $(SRC:.c=.o)
DEPS = $(OBJS:.o=.d)

CC = gcc

%.o: %.c Makefile
	$(CC) $(FLAGS) -c $< -o $@

all: make_libs $(NAME)
	@echo "\033[1;32mThe project has been compiled successfully\033[0m"

configure_readline:
	cd $(READLINE) && ./configure

make_libs:
	@if [ ! -f "$(READLINE)/libreadline.a" ]; then \
		echo "Configuring and building Readline..."; \
		$(MAKE) configure_readline; \
		$(MAKE) -C $(READLINE); \
	else \
		echo "Readline is already up to date."; \
	fi
	$(MAKE) -C $(LIBFT)
	@echo "\033[1;32mLibraries have been compiled successfully\033[0m"


$(NAME): $(OBJS) $(HEADER) Makefile
	@$(CC) $(FLAGS) $(OBJS) -L$(LIBFT) -lft -L$(READLINE) -lhistory -lreadline -ltermcap -o $@

clean:
	$(RM) $(OBJS) $(DEPS)
	$(MAKE) -C $(LIBFT) clean
	$(MAKE) -C $(READLINE) clean

fclean: clean
	$(RM) $(NAME)
	$(MAKE) -C $(LIBFT) fclean
	$(MAKE) -C $(READLINE) clean

re: fclean all

.PHONY: re clean fclean all configure_readline

-include $(DEPS)

