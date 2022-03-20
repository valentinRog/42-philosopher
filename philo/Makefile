NAME = philo

SRC = $(wildcard src/*.c)

INCLUDES = includes

OBJ = $(SRC:.c=.o)

CFLAGS = -Wall -Wextra -Werror  #-fsanitize=thread

CC = gcc

$(RM) = rm -rf

.c.o: $(SRC) $(SRCB)
	$(CC) $(CFLAGS) -I $(INCLUDES) -c -o $@ $<

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -lpthread -o $@

all: $(NAME)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

run: $(NAME)
	./$(NAME) 5 600 300 200

re: fclean all
