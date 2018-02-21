.SILENT:

CC	= g++

DEBUG	?= 0

RM	= rm -rf

LDFLAGS	+= -lpthread

CXXFLAGS += -W -Wall -Iinc -std=c++17

ifeq ($(DEBUG), 1)
	CXXFLAGS+= -DDEBUG -g3
else
	# CXXFLAGS+= -Werror 
endif

SRCS	= 	src/main.cpp \
	src/Main.cpp \
	src/Worker.cpp \
	src/Config.cpp \
	src/ModuleManager.cpp \
	src/socket/Handler.cpp \
	src/socket/Listener.cpp \
	src/socket/Reactor.cpp \
	src/socket/Session.cpp \
	src/socket/Stream.cpp \
	src/http/RequestHandler.cpp \
	src/http/HttpParser.cpp \
	src/Server.cpp

OBJS	= $(SRCS:.cpp=.o)

NAME	= zia

all: $(NAME) ## Compile

$(NAME): $(OBJS)
	$(CC) -o $(NAME) $(OBJS) $(LDFLAGS)

clean: ## Clean .o
	$(RM) $(OBJS)

fclean: clean ## Clean binary and .o
	$(RM) $(NAME)
	$(RM) debug

clean_comment: ## Clean all comment //
	find -type f -name "*.cpp" | xargs sed -i 's://.*28198::g'

help: 
	@grep -E '(^[a-zA-Z_-]+:.*?##.*$$)|(^##)' Makefile | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[32m%-30s\033[0m %s\n", $$1, $$2}' | sed -e 's/\[32m##/[33m/'

debug: fclean ## Compile to debug mode
	make re DEBUG=1
	mkdir debug
	mv $(NAME) debug/

re: fclean all ## Recompile

.PHONY: all re clean fclean help debug clean_comment
