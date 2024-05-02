NAME = project

COMPILER = c++
# CFLAGS = -Wall -Wextra -Werror

SRCS =	src/main.cpp \
		src/game.cpp \
		src/parcing.cpp

FLAGS = -lsfml-graphics -lsfml-window -lsfml-system
# -framework OpenGL

all: $(NAME)

$(NAME):
	$(COMPILER) $(CFLAGS) ${SRCS} -o $(NAME) $(FLAGS)

clean:
	rm -rf $(NAME)

fclean: clean

re: fclean all

.PHONY: all clean fclean re