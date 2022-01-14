
NAME = pipex

SRCS = test.c

OBJ = $(SRCS:.c=.o)

FLAGS = -Wall -Wextra -Werror

all: $(NAME)

%.o: %.c pipex.h
	$(CC)  $(FLAGS) -c $< -o $@ -I pipex.h

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