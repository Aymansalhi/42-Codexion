NAME := codexion

SRC := src/main.c src/parsing.c utils/pars_utils.c 

OBJ := $(SRC:.c=.o)

CFLAGS := -g -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c include/codexion.h
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -rf $(OBJ)

fclean:
	rm -rf $(OBJ) $(NAME)

re: fclean all

.PHONY: all clean fclean re