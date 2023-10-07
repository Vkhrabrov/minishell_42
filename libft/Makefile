# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vkhrabro <vkhrabro@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/19 19:57:42 by vkhrabro          #+#    #+#              #
#    Updated: 2023/07/11 00:30:45 by vkhrabro         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #



NAME = libft.a
HEADER = libft.h

SRC_F:= ft_isprint ft_isascii ft_isalnum ft_isalpha ft_isdigit ft_strlen ft_memset ft_bzero ft_memcpy ft_memmove ft_strlcpy ft_strlcat ft_toupper ft_tolower ft_strchr ft_strrchr ft_strncmp ft_memchr ft_memcmp ft_strnstr ft_atoi ft_calloc ft_strdup ft_substr ft_strjoin ft_strtrim ft_split ft_itoa ft_strmapi ft_striteri ft_putchar_fd ft_putstr_fd ft_putendl_fd ft_putnbr_fd ft_putchar

SRC_BNS_F:= ft_lstnew ft_lstadd_front ft_lstsize ft_lstlast ft_lstadd_back ft_lstdelone ft_lstclear ft_lstiter ft_lstmap

FLAGS = -Wall -Wextra -Werror
RM = rm -f
SRC = $(addsuffix .c, $(SRC_F))
OBJS = $(SRC:.c=.o)
SRC_BNS = $(addsuffix .c, $(SRC_BNS_F))
OBJS_BNS = $(SRC_BNS:.c=.o)

CC = gcc

%.o: %.c $(HEADER)
	@$(CC) $(FLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJS) $(OBJS_BNS) $(HEADER)
	@ar -rcs $(NAME) $(OBJS) $(OBJS_BNS)
	@touch $@

clean:
	$(RM) $(OBJS) $(OBJS_BNS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: re clean fclean all
