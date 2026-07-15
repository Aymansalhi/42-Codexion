NAME := codexion

SRC :=

OBJ :=

CFLAGS := -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c headerfile.h
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -rf $(OBJ)

fclean:
	rm -rf $(OBJ) $(OBJ)

re: fclean all

.PHONY: fclean all