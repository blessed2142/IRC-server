#==================================>COLORS<====================================#

_RED		=	\e[31m
_YELLOW		=	\e[33m
_GREEN		=	\e[32m
_END		=	\e[0m

#==================================>FILES<=====================================#

SRCS		=	main.cpp \
				channel.cpp \
				client.cpp \
				handle_command.cpp \
				server.cpp

SRCS_BOT	=	botislav.cpp

#===============================>COMPILATIONS<=================================#

CLANG		=	clang++

CFLAGS		=	-Wall -Werror -Wextra -g -fsanitize=address 

#===================================>DELETE<===================================#

RM			=	rm -rf

#=================================>DIRECTORIES<================================#

DIR_HEAD	=	./includes

#===============================>COMPILED_SOURCES<=============================#

OBJS		=	${SRCS:%.cpp=%.o}

BOT_OBJS	=	${SRCS_BOT:%.cpp=%.o}

NAME		=	ircserv

BOT_NAME	=	bot

#================================>COMPILED_RULES<==============================#

all:		${NAME}

${NAME}:	${OBJS}
			@${CLANG} ${CFLAG} -o ${NAME} ${OBJS}
			@printf "\033[2K\r${_GREEN} IRCserv create: '${NAME}'. ${_END}✅\n"

bot:		${BOT_OBJS}
			@${CLANG} ${CFLAG} -o ${BOT_NAME} ${BOT_OBJS}
			@printf "\033[2K\r${_GREEN} BOT create: '${BOT_NAME}'. ${_END}✅\n"

#===========================>COMPILED_SOURCES_RULES<===========================#

%.o:		%.cpp
			@${CLANG} ${CFLAG} -I ${DIR_HEAD} -c $< -o $@
			@printf "\033[2K\r${_YELLOW} Compiling $< ${_END}⌛"		

#====================================>CLEAN_RULES<=============================#

clean:
			@rm -f ${OBJS} ${BOT_OBJS}
			@printf "\033[2K\r${_RED} '".o"' has been deleted. ${_END}🗑️\n"

fclean:		clean
			@rm -f ${NAME} ${BOT_NAME}
			@printf "\033[2K\r${_RED} '"${NAME}"' and '"${BOT_NAME}"' has been deleted. ${_END}🗑️\n"

re:			fclean all

.PHONY: all clean fclean re