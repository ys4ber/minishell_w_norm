NAME = minishell

CC = gcc


CFLAGS = -Wall -Wextra -Werror 

# SEGV = -g3 -fsanitize=address


SRCS = main.c ft_split.c ft_store.c ft_utils.c ft_quotes.c\
	ft_functions.c ft_exc.c ft_builtins.c ft_syntax_error.c ft_execredirction.c\
	get_next_line.c 

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME) : $(OBJS)
	$(CC)  $(CFLAGS)  $(SRCS) -o $(NAME)  -lreadline 

clean:
	rm -rf $(OBJS)
	
fclean:
	rm -rf $(OBJS) $(NAME)

re: fclean all clean

.PHONY: all clean fclean re