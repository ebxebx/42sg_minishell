NAME      = minishell

TEST_CMD  = ./$(NAME) "debug"

LIBFT_DIR = ./libft
LIBFT     = $(LIBFT_DIR)/libft.a

SRCS = 	minishell.c minishell_utils.c \
		minishell_debug.c
OBJS = $(SRCS:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror -I$(LIBFT_DIR) -g -lreadline
LIBFT_FLAGS = -L$(LIBFT_DIR) -lft

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

test2: $(NAME)
	valgrind --leak-check=full $(TEST_CMD)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(DEPFLAGS) $(OBJS) $(LIBFT_FLAGS) -o $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

# The Pattern Rule. 
%.o: %.c
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

# include dependency files if they exist (don't error on first run)
-include $(DEPS)

clean:
	rm -f $(OBJS) $(BONUS_OBJS) $(DEPS) $(BONUS_DEPS)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME) $(BONUS)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
