# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tvithara <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/22 14:12:12 by tvithara          #+#    #+#              #
#    Updated: 2024/12/22 14:12:15 by tvithara         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

CC = cc
CFLAGS = -Wall -Werror -Wextra 
AR = ar rcs
RM = rm -f

# Directories
OBJ_DIR		= obj
INC_DIR		= includes
LIB_DIR		= lib
PRINTF_DIR	= printf
GNL_DIR		= get_next_line

# Source files
LIB_SRC	= ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c \
			  ft_strlen.c ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c \
			  ft_strlcpy.c ft_strlcat.c ft_toupper.c ft_tolower.c ft_strchr.c \
			  ft_strrchr.c ft_strncmp.c ft_memchr.c ft_memcmp.c ft_strnstr.c \
			  ft_atoi.c ft_calloc.c ft_strdup.c ft_substr.c ft_strjoin.c \
			  ft_strtrim.c ft_split.c ft_itoa.c ft_strmapi.c ft_striteri.c \
			  ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c

PRINTF_SRC	= ft_printf.c ft_printf_utils.c ft_print_hex.c ft_print_int.c \
			  ft_print_p.c ft_print_u.c

GNL_SRC		= get_next_line.c get_next_line_utils.c 

BONUS_SRC	= ft_lstnew.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c \
			  ft_lstadd_back.c ft_lstdelone.c ft_lstclear.c ft_lstiter.c ft_lstmap.c

# Add directory prefixes
LIB_SRC     := $(addprefix $(LIB_DIR)/, $(LIB_SRC))
PRINTF_SRC  := $(addprefix $(PRINTF_DIR)/, $(PRINTF_SRC))
GNL_SRC     := $(addprefix $(GNL_DIR)/, $(GNL_SRC))
BONUS_SRC   := $(addprefix $(LIB_DIR)/, $(BONUS_SRC))

# All sources
SRC         = $(LIB_SRC) $(PRINTF_SRC) $(GNL_SRC)

# Objects (mantengo le sottocartelle dentro obj/)
OBJ         = $(SRC:%.c=$(OBJ_DIR)/%.o)
BONUS_OBJ   = $(BONUS_SRC:%.c=$(OBJ_DIR)/%.o)

# Includes
INC         = -I$(INC_DIR)

# Colors for output
GREEN	= \033[0;32m
ORANGE = \033[38;5;208m
BLUE	= \033[0;34m
RESET	= \033[0m

# Main target
all: $(NAME)

$(NAME): $(OBJ)
	@$(AR) $@ $^
	@echo "$(GREEN)libft compiled successfully!$(RESET)"

# Bonus target
bonus: $(OBJ) $(BONUS_OBJ)
	@$(AR) $(NAME) $^
	@echo "$(BLUE)libft bonus compiled successfully!$(RESET)"

# Object file compilation
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INC) -c $< -o $@
#	@echo "Compiling: $<" 	 Uncomment for verbose output

# Clean targets
clean:
	@$(RM) -r $(OBJ_DIR)
	@echo "$(ORANGE)libft object files removed$(RESET)"

fclean: clean
	@$(RM) $(NAME)
	@echo "$(ORANGE)libft removed$(RESET)"

re: fclean all

.PHONY: all clean fclean re
