CC	= g++

DEBUG	?= 0

RM	= rm -rf

LDFLAGS	+= -shared -lboost_filesystem

CXXFLAGS += -W -Wall -std=c++17 -fPIC  -rdynamic -I ../../inc -g3

ifeq ($(DEBUG), 1)
	CXXFLAGS+= -DDEBUG -g3
else
	CXXFLAGS+= -g
endif

SRCS	= 	FileReader.cpp
OBJS	= $(SRCS:.cpp=.o)

NAME	= file_reader.so

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
