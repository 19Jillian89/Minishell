# **************************************************************************** #
#                                   COLORS                                    #
# **************************************************************************** #

GREEN=\033[0;32m
RED=\033[0;31m
YELLOW=\033[0;33m
NC=\033[0m

# **************************************************************************** #
#                                 PROJECT NAME                                #
# **************************************************************************** #

NAME = minishell

# **************************************************************************** #
#                                  DIRECTORIES                                 #
# **************************************************************************** #

SRC_DIR = .
OBJ_DIR = obj
INC_DIR = includes
LIBFT_DIR = libft

# **************************************************************************** #
#                                    FILES                                     #
# **************************************************************************** #

# PARSER
PARSER_SRCS = parser/lexer.c \
              parser/parser.c \
              parser/utils.c \
              parser/quote_handler.c \
              parser/utils2.c

# BUILDER
BUILDER_SRCS = builder/builder_cmd.c \
               builder/builder_free.c \
               builder/builder_redirs.c \
               builder/builder_utils.c

# BUILTIN
BUILTIN_SRCS = builtin/builtin.c \
               builtin/cd.c \
               builtin/echo.c \
               builtin/env.c \
               builtin/env_utility.c \
               builtin/exit.c \
               builtin/export.c \
               builtin/export_utility.c \
               builtin/export_utility2.c \
               builtin/pwd.c \
               builtin/unset.c

# EXECUTION
EXECUTION_SRCS = execution/exec_single.c \
                 execution/execute_pipeline.c \
                 execution/find_path.c \
                 execution/heredoc_child.c \
                 execution/heredoc_fork.c \
                 execution/heredoc_handler.c \
                 execution/heredoc_utils.c \
                 execution/pipe_child.c \
                 execution/pipe_fds.c \
                 execution/pipe_wait.c \
                 execution/redirections.c \
                 execution/run_pipeline.c

# SIGNALS
SIGNAL_SRCS = signals/signals.c

# ALL SRC
SRC =  main.c \
       $(PARSER_SRCS) \
       $(BUILDER_SRCS) \
       $(BUILTIN_SRCS) \
       $(EXECUTION_SRCS) \
       $(SIGNAL_SRCS)

OBJ = $(SRC:%.c=$(OBJ_DIR)/%.o)

# **************************************************************************** #
#                                  COMPILATION                                 #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Wextra -Werror \
         -I$(INC_DIR) -I$(LIBFT_DIR)/includes

# **************************************************************************** #
#                                    RULES                                     #
# **************************************************************************** #

all: $(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/parser
	@mkdir -p $(OBJ_DIR)/builder
	@mkdir -p $(OBJ_DIR)/builtin
	@mkdir -p $(OBJ_DIR)/execution
	@mkdir -p $(OBJ_DIR)/signals

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(GREEN)Compiled: $<$(NC)"

$(LIBFT_DIR)/libft.a:
	@echo "$(YELLOW)Compiling libft...$(NC)"
	@make -C $(LIBFT_DIR)

$(NAME): $(OBJ) $(LIBFT_DIR)/libft.a
	@$(CC) $(CFLAGS) $(OBJ) $(LIBFT_DIR)/libft.a -lreadline -o $(NAME)
	@echo "$(GREEN)Minishell compiled successfully!$(NC)"

clean:
	@rm -rf $(OBJ_DIR)
	@make -C $(LIBFT_DIR) clean
	@echo "$(RED)Object files removed.$(NC)"

fclean: clean
	@rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean
	@echo "$(RED)Executable removed.$(NC)"

re: fclean all

.PHONY: all clean fclean re


