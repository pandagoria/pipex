
NAME = pipex

SRCS = main.c utils.c parse_cmds_path.c \

OBJ = $(SRCS:.c=.o)

FLAGS = -Wall -Wextra -Werror

all: $(NAME)

%.o: %.c pipex.h Makefile
	$(CC)  $(FLAGS) -c $< -o $@

$(NAME): $(OBJ)
	make -C ./libft/
	$(CC) $(OBJ) ./libft/libft.a $(FLAGS) -o $(NAME)

re: fclean all

clean:
	make clean -C ./libft/
	rm -rf *.o

fclean: clean
	rm -rf $(NAME)
	make fclean -C  ./libft/

.PHONY: cat clean fclean re all