NAME      = minishell

TEST_CMD  = ./$(NAME) "debug"
TEST1_CMD  = ./$(NAME)

LIBFT_DIR = ./libft
LIBFT     = $(LIBFT_DIR)/libft.a
PRINTF_FLOAT ?= 0
LIBFT_MAKE = $(MAKE) -C $(LIBFT_DIR) PRINTF_FLOAT=$(PRINTF_FLOAT)

SRCS =	minishell.c minishell_init.c minishell_exec_command.c \
		env/env_modify_entry.c env/env_modify_env+export.c env/env_utils.c \
		parsing/tokenize.c parsing/ast.c \
		executor/execute_ast.c executor/execute_command_node.c \
		executor/execute_pipeline.c executor/redirection.c \
		executor/execute_utils.c executor/execute_builtin.c \
		executor/execute_command_execve.c \
		executor/execute_heredoc.c \
		executor/execute_heredoc_preprocess.c \
		executor/execute_heredoc_utils.c executor/execute_heredoc_utils2.c \
		builtin/builtin_echo.c builtin/builtin_env.c builtin/builtin_pwd.c \
		builtin/builtin_export.c builtin/builtin_unset.c builtin/builtin_cd.c \
		builtin/builtin_exit_utils.c builtin/builtin_exit.c \
		expander/expand_string.c expander/expand_token.c expander/expand_utils.c \
		expander/expand_heredoc.c expander/expand.c \
		minishell_signal.c \
		minishell_prompt.c \
		minishell_debug.c
OBJS = $(SRCS:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror -g 
LIB_FLAGS = -L$(LIBFT_DIR) -lft -lreadline
ifeq ($(PRINTF_FLOAT),1)
	LIB_FLAGS += -lm
endif

# Add dependency flags and derived files
DEPFLAGS = -MMD -MP
DEPS = $(SRCS:.c=.d)

ifeq ($(DEBUG),1)
	CFLAGS += -DDEBUG=1 -g3
else
	CFLAGS += -DDEBUG=0
endif

all: $(NAME)

test: $(NAME)
	$(TEST_CMD)

test1: $(NAME)
	$(TEST1_CMD)

testv: $(NAME)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes --trace-children=yes --suppressions=minishell_readline_merged.supp $(TEST_CMD)

tester: $(NAME)
	cd minishell_tester/ && ./tester

testerv: $(NAME)
	cd minishell_tester/ && ./tester valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(DEPFLAGS) $(OBJS) $(LIB_FLAGS) -o $(NAME)

$(LIBFT):
	$(LIBFT_MAKE)

# The Pattern Rule. 
$(OBJS): %.o : %.c
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

# include dependency files if they exist (don't error on first run)
-include $(DEPS)

clean:
	rm -f $(OBJS) $(BONUS_OBJS) $(DEPS) $(BONUS_DEPS)
	$(LIBFT_MAKE) clean

fclean: clean
	rm -f $(NAME) $(BONUS)
	$(LIBFT_MAKE) fclean

re: fclean all

.PHONY: all clean fclean re
