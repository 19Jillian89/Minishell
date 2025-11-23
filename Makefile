NAME        = minishell

CC          = cc
CFLAGS      = -Wall -Wextra -Werror -I. -Ilibft
AR          = ar rcs
RM          = rm -f

LIBFT_DIR   = libft
LIBFT       = $(LIBFT_DIR)/libft.a

# Parser 
PARSER_SRCS = Parser/lexer.c \
              Parser/parser.c \
              Parser/utils.c \
			  Parser/quote_handler.c \
              Parser/main.c

# Executor
# EXEC_SRCS   = Executor/exec.c ...

SRCS        = $(PARSER_SRCS)
# SRCS      += $(EXEC_SRCS)

OBJ_DIR     = obj
OBJS        = $(SRCS:%.c=$(OBJ_DIR)/%.o)

RL_FLAGS    = -lreadline


all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(RL_FLAGS) -o $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

clean:
	$(RM) $(OBJS)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	$(RM) $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

# Build di debug (gdb / sanitizer)
DEBUGFLAGS  = -g3 -fsanitize=address -fsanitize=undefined

debug: CFLAGS += $(DEBUGFLAGS)
debug: re

.PHONY: all clean fclean re debug