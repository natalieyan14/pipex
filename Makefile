NAME = pipex

CC = cc
CFLAGS = -Wall -Wextra -Werror # -fsanitize=address
LIBFT = libft/libft.a

SRC = src/pipex.c src/pipex_utils.c 

all: $(NAME)

$(NAME): $(LIBFT) $(SRC) includes/pipex.h
	$(CC) $(CFLAGS) -Iincludes $(SRC) $(LIBFT) -o $(NAME)

$(LIBFT):
	@make -C libft/

clean:
	@make clean -C libft/

%.o: %.c Makefile 
		cc $(FLAGS) -c $< -o $@

fclean: clean
	rm -f $(NAME)
	@make fclean -C libft/

re: fclean all

.PHONY: all clean fclean re