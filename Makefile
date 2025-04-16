##
## EPITECH PROJECT, 2025
## rush
## File description:
## Main Makefile
##

SERVER_NAME	=	jetpack_server
CLIENT_NAME	=	jetpack_client

TESTS_NAME	=	unit_tests.out

SERVER_DIR	=	./server/
CLIENT_DIR	=	./client/
GL		=	./sources/globals/
MISC		=	./sources/miscellaneous/
TESTS		=	./tests/

SERVER_MAIN	=	server/main.c
SERVER_SRC	=	\
	$(SERVER_DIR)handle_clients.c	\
	$(SERVER_DIR)loop.c						\
	$(SERVER_DIR)parse_cmd_args.c	\
	$(SERVER_DIR)receive_command.c\
	$(SERVER_DIR)send_commands.c	\
	$(SERVER_DIR)set_server.c			\
	$(SERVER_DIR)set_signal.c			\
	# $(SERVER_DIR)game_logic.c			\

CLIENT_MAIN	=	client/main.cpp
CLIENT_SRC	=	\
	$(CLIENT_DIR)assetManager.cpp	\
	$(CLIENT_DIR)commandHandler.cpp	\
	$(CLIENT_DIR)gameRenderer.cpp	\
	$(CLIENT_DIR)gameState.cpp	\
	$(CLIENT_DIR)networkManager.cpp	\
	$(CLIENT_DIR)player.cpp	\

TESTS_SRC	=	\

#Headers folder
INCLUDES	=	includes/	\


ERROR	=	-Wall -Wextra -Wshadow #-Werror
CFLAGS	+=	$(ERROR) -I$(INCLUDES) -g
CXXFLAGS+=	$(ERROR) -I$(INCLUDES) -g -std=c++11

CC	:=	gcc
CXX	:=	g++

SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

SERVER_OBJ	=	$(SERVER_SRC:.c=.o) $(SERVER_MAIN:.c=.o)
CLIENT_OBJ	=	$(CLIENT_SRC:.cpp=.o) $(CLIENT_MAIN:.cpp=.o)

DEP	=	$(SERVER_SRC:.c=.d)	\
		$(CLIENT_SRC:.cpp=.d)	\
		$(SERVER_MAIN:.c=.d)	\
		$(CLIENT_MAIN:.cpp=.d)

LIBS	=	./libs/
LLIBS	=	-L$(LIBS)

FLAGS	=	$(CFLAGS) $(LLIBS)
UNIT_FLAGS	=	$(FLAGS) -lcriterion --coverage

all:	$(SERVER_NAME) $(CLIENT_NAME)

$(SERVER_NAME): $(SERVER_OBJ)
	$(CC) -o $(SERVER_NAME) $(SERVER_OBJ) $(FLAGS)

$(CLIENT_NAME): $(CLIENT_OBJ)
	$(CXX) -o $(CLIENT_NAME) $(CLIENT_OBJ) $(CXXFLAGS) $(SFML_LIBS)

server: $(SERVER_NAME)

client: $(CLIENT_NAME)

-include $(DEP)
%.o: %.c
	$(CC) -c $< -o $@ -MMD -MF $*.d -MT $@ $(CFLAGS)

%.o: %.cpp
	$(CXX) -c $< -o $@ -MMD -MF $*.d -MT $@ $(CXXFLAGS)

setup:
	@mkdir -p ./client
	@mkdir -p ./includes/client
	@mkdir -p ./assets

clean:
	rm -f $(SERVER_OBJ)
	rm -f $(CLIENT_OBJ)
	rm -f $(DEP)
	rm -f *.gcno
	rm -f *.gcda
	rm -f src/*.gcno src/*.gcda
	rm -f src/commands/*.gcno src/commands/*.gcda
	rm -f tests/*.gcno tests/*.gcda
	rm -f vgcore.*
	rm -f tests/*.o
	rm -f tests/*.d

libclean: clean

fclean:	libclean
	rm -f $(SERVER_NAME) $(CLIENT_NAME)
	rm -f $(TESTS_NAME)
	find -name "lib*.a" -delete

re:	fclean all

tests_run: fclean
	$(CC) -o $(TESTS_NAME) $(SRC) $(TESTS_SRC) $(UNIT_FLAGS)
	-./$(TESTS_NAME) --verbose
	gcovr --exclude tests/
	gcovr --exclude tests/ --branches

.PHONY:	all clean libclean fclean re \
	remake tests_run unit_tests gcovr setup server client
