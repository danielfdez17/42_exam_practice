NAME = microshell

CCW = cc -Wall -Werror -Wextra

all:
	$(CCW) main.c -o $(NAME)

run: all
	./$(NAME) /bin/ls "|" /usr/bin/grep microshell ";" /bin/echo i love my microshell

fclean:
	rm ./$(NAME)

debug: fclean
	$(CCW) main.c -g3 -o $(NAME)
	gdb ./$(NAME)

valgrind: fclean all
	valgrind ./$(NAME)

original:
	$(CCW) original.c -o $(NAME)
	./$(NAME) /bin/ls "|" /usr/bin/grep microshell ";" /bin/echo i love my microshell