.SILENT:

CC	= g++

DEBUG	?= 0

RM	= rm -rf

LDFLAGS	+= -shared

CXXFLAGS += -W -Wall -Iinc -I../inc -fPIC -rdynamic -std=c++17

SRCS	= 	src/DefaultModule.cpp \

OBJS	= $(SRCS:.cpp=.o)

NAME	= response.so

all: $(NAME) ## Compile

$(NAME): $(OBJS)
	$(CC) -o $(NAME) $(OBJS) $(LDFLAGS)

clean: ## Clean .o
	$(RM) $(OBJS)

fclean: clean ## Clean binary and .o
	$(RM) $(NAME)
	$(RM) debug

clean_comment: ## Clean all comment //
	find -type f -name "*.cpp" | xargs sed -i 's://.*15212::g'

help: 
	@grep -E '(^[a-zA-Z_-]+:.*?##.*$$)|(^##)' Makefile | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[32m%-30s\033[0m %s\n", $$1, $$2}' | sed -e 's/\[32m##/[33m/'

re: fclean all ## Recompile

.PHONY: all re clean fclean help debug clean_comment
