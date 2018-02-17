CC	= g++

DEBUG	?= 0

RM	= rm -rf

LDFLAGS	+= -shared -rdynamic

CXXFLAGS += -W -Wall -Iinclude -fPIC -c -std=c++17

ifeq ($(DEBUG), 1)
	CXXFLAGS+= -DDEBUG -g3
else
	CXXFLAGS+= -Werror 
endif

SRCS	= sources/DefaultModule.cpp

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

re: fclean all ## Recompile

.PHONY: all re clean fclean help debug clean_comment
