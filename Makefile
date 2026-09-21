# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hclaude <hclaude@student.42mulhouse.fr>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/09/21 16:06:51 by hclaude           #+#    #+#              #
#    Updated: 2026/09/21 16:06:51 by hclaude          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_malcolm

CC = gcc

CFLAGS = -Wall -Wextra -Werror -g3

OBJDIR = .objs

LIBFT_DIR = lib/turbo_libft
LIBFT = $(LIBFT_DIR)/libft.a

INCLUDE = -Ilib -I$(LIBFT_DIR)

SRCFILES = src/main.c \
src/parsing.c \
src/hex_utils.c \
src/print_data.c \
src/errors.c \
src/utils.c

OBJS = $(patsubst src/%.c, $(OBJDIR)/%.o, $(SRCFILES))

all : $(NAME)

$(OBJDIR)/%.o : src/%.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(LIBFT) :
	@$(MAKE) -C $(LIBFT_DIR)

$(NAME) : $(LIBFT) $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)
	@echo "\033[32m$(NAME) compiled\033[0m"

clean :
	@rm -rf $(OBJDIR)
	@$(MAKE) -C $(LIBFT_DIR) clean
	@echo "\033[31mclean $(NAME)\033[0m"

fclean : clean
	@rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean

re : fclean all

.PHONY : all clean fclean re
