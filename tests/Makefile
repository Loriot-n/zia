.SILENT:

NAME		= zia_test

CXX			= g++
LDFLAGS		+= -Lbuild/ -lgtest -lgtest_main -pthread
CXXFLAGS	+= -W -Wall -Wextra -Igoogletest/googletest/include/

SRCS	= main.cpp
OBJS	= $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS) lib
	$(CXX) -o $(NAME) $(OBJS) $(LDFLAGS) $(CXXFLAGS)

lib:
	echo "Building GoogleTest librairies."
	make -C build --no-print-directory 
	echo "Done."

clean:
	rm -rf $(OBJS)
	make -C build clean --no-print-directory

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all lib clean fclean re
