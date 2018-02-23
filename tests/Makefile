.SILENT:

include ../Makefile.common

NAME		= zia_test

CXX			= g++
LDFLAGS		+= -Lbuild/ -lgtest -lgtest_main -pthread
CXXFLAGS	+= -W -Wall -Wextra -Igoogletest/googletest/include/ -I../inc -g

SRC = $(addprefix ../, $(SRCS))
SRCTEST	+= $(filter-out ../src/main.cpp ../src/Main.cpp, $(SRC)) main.cpp
OBJS	= $(SRCTEST:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS) lib
	$(CXX) -o $(NAME) $(OBJS) $(LDFLAGS) $(CXXFLAGS)
	echo "$(NAME) done."

lib:
	echo "Building GoogleTest librairies."
	make -C build --no-print-directory 
	echo "GoogleTest librairies done."

clean:
	rm -rf $(OBJS)
	make -C build clean --no-print-directory

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all lib clean fclean re
