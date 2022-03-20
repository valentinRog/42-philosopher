NAME = philo

SRC = $(wildcard src/*.c)

INCLUDES = includes

OBJ = $(SRC:.c=.o)

CFLAGS = #-Wall -Wextra -Werror

CC = gcc

$(RM) = rm -rf

.c.o: $(SRC) $(SRCB)
	$(CC) $(CFLAGS) -fsanitize=thread -lpthread -I $(INCLUDES) -c -o $@ $<

$(NAME): $(OBJ)
	$(CC) $(OBJ) -fsanitize=thread -lpthread -o $@

all: $(NAME)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

run: $(NAME)
	./$(NAME) 50 600 300 200

re: fclean all
