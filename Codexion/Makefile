NAME := codexion

SRC := src/main.c src/parsing.c utils/parsing_utils.c src/exit_clean.c \
		src/initialization/init.c src/initialization/init_queue.c \
		src/threads/create_thread.c src/threads/coder.c \
		src/threads/monitor.c src/queue/fifo_queue.c utils/time_utils.c utils/coder_utils.c \
		utils/dongel_managment.c src/queue/edf_queue.c  src/queue/priority_queue3.c\
		src/queue/priority_queue.c src/queue/priority_queue2.c utils/coder_utils_2.c

OBJ := $(SRC:.c=.o)

CFLAGS := -g -Wall -Wextra -Werror -pthread -fsanitize=address

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	@printf '%s\n' \
	'┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓' \
	'┃ Usage:                                                                                   ┃' \
	'┃ ./codexion <number_of_coders> <time_to_burnout> <time_to_compile> <time_to_debug>        ┃' \
	'┃            <time_to_refactor> <number_of_compiles_required> <dongle_cooldown> <scheduler>┃' \
	'┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛'

%.o: %.c include/codexion.h
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -rf $(OBJ)

fclean:
	rm -rf $(OBJ) $(NAME)

re: fclean all

test: $(NAME)
	bash edf_test.sh ./$(NAME)

.PHONY: all clean fclean re