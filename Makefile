##
## EPITECH PROJECT, 2025
## rush
## File description:
## Main Makefile
##

#Executable names
SERVER_NAME	=	jetpack_server
CLIENT_NAME	=	jetpack_client


TESTS_NAME	=	unit_tests.out

# Folders name
SERVER_DIR	=	./server/
GL	=	./sources/globals/
MISC	=	./sources/miscellaneous/
TESTS	=	./tests/

#Sources
SERVER_MAIN	=	server/main.c
SERVER_SRC	=	\
	$(SERVER_DIR)set_server.c	\
	$(SERVER_DIR)parse_map.c	\
	$(SERVER_DIR)loop.c	\
	$(SERVER_DIR)handle_clients.c	\
	$(SERVER_DIR)receive_command.c	\
	$(SERVER_DIR)send_commands.c	\
	$(SERVER_DIR)connections_utils.c	\
	$(SERVER_DIR)free_all.c	\
	$(SERVER_DIR)game_logic.c	\

CLIENT_MAIN	=	client/main.cpp
CLIENT_SRC	=	\

TESTS_SRC	=	\

#Headers folder
INCLUDES	=	-I./includes/	\


#GCC Flags
ERROR	=	-Wall -Wextra -Wshadow #-Werror
#Compilation Flags
CFLAGS	+=	$(ERROR) -I$(INCLUDES) -g
#Pre Compilation
CC	:=	gcc
CP	:=	g++

OBJ	=	$(SERVER_SRC:.c=.o)	\
		$(CLIENT_SRC:.cpp=.o)
DEP	=	$(SERVER_SRC:.c=.d)	\
		$(CLIENT_SRC:.cpp=.d)	\
		$(MAIN:.c=.d)

#Libraries
LIBS	=	./libs/
LLIBS	=	-L$(LIBS)
LIB_	=	#$(LIBS)put a path to a library here



#Compilation Flags
FLAGS	=	$(CFLAGS) $(LLIBS)
#Test Compilation Flags
UNIT_FLAGS	=	$(FLAGS) -lcriterion --coverage	-lSDL2

all:	$(SERVER_NAME) #$(CLIENT_NAME)

$(SERVER_NAME): $(OBJ) $(SERVER_MAIN:.c=.o) $(LIB_)
	$(CC) -o $(SERVER_NAME) $(OBJ) $(SERVER_MAIN:.c=.o) $(FLAGS)

$(CLIENT_NAME): $(OBJ) $(CLIENT_MAIN:.cpp=.o) $(LIB_)
	$(CC) -o $(CLIENT_NAME) $(OBJ) $(CLIENT_MAIN:.cpp=.o) $(FLAGS)

-include $(DEP)
%.o: %.c
	$(COMPILE.c) $< -o $@ -MMD -MF $*.d -MT $@ $(CFLAGS)


clean:
	rm -f $(OBJ)
	rm -f $(SERVER_MAIN:.c=.o)
	rm -f $(CLIENT_MAIN:.cpp=.o)
	rm -f $(DEP)
	rm -f server/main.d
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
	rm -f $(NAME)
	rm -f $(TESTS_NAME)
	find -name "lib*.a" -delete

re:	fclean all

tests_run: fclean
	$(CC) -o $(TESTS_NAME) $(SRC) $(TESTS_SRC) $(UNIT_FLAGS)
	-./$(TESTS_NAME) --verbose
	gcovr --exclude tests/
	gcovr --exclude tests/ --branches

.PHONY:	all clean	libclean	\
		fclean	re	remake		\
		tests_run	unit_tests	gcovr
