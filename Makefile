include Makefile.common

DEBUG	?= 0

RM	= rm -rf

LDFLAGS	+= -ljsoncpp -ldl

LDFLAGS	+= -lpthread

CXXFLAGS += -W -Wall -Iinc  -std=c++17

ifeq ($(DEBUG), 1)
	CXXFLAGS+= -DDEBUG -g3 -Wfatal-errors
else
	CXXFLAGS+= -Werror
endif

OBJS	= $(SRCS:.cpp=.o)

NAME	= zia

all: $(NAME) ## Compile

$(NAME): $(OBJS)
	$(CC) -o $(NAME) $(OBJS) $(LDFLAGS)

clean: clean_modules ## Clean .o
	$(RM) $(OBJS)

fclean: fclean_modules clean ## Clean binary and .o
	$(RM) $(NAME)
	$(RM) debug

modules:
	$(MAKE) -C modules/request
	$(MAKE) -C modules/file_reader
	$(MAKE) -C modules/log
	$(MAKE) -C modules/response
	$(MAKE) -C modules/cgi

clean_modules:
	$(MAKE) clean -C modules/request
	$(MAKE) clean -C modules/file_reader
	$(MAKE) clean -C modules/log
	$(MAKE) clean -C modules/response
	$(MAKE) clean -C modules/cgi

fclean_modules:
	$(MAKE) fclean -C modules/request
	$(MAKE) fclean -C modules/file_reader
	$(MAKE) fclean -C modules/log
	$(MAKE) fclean -C modules/response
	$(MAKE) fclean -C modules/cgi

clean_comment: ## Clean all comment //
	find -type f -name "*.cpp" | xargs sed -i 's://.*15451::g'

help:
	@grep -E '(^[a-zA-Z_-]+:.*?##.*$$)|(^##)' Makefile | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[32m%-30s\033[0m %s\n", $$1, $$2}' | sed -e 's/\[32m##/[33m/'

debug: fclean ## Compile to debug mode
	make -j re DEBUG=1
	mkdir debug
	# make -j -C default_module/
	# cp default_module/response.so ./Modules/response/
	#make -C modules/Php-cgi
	mv $(NAME) debug/

re: fclean all ## Recompile

%.o:		%.cpp
		$(CXX) $(CXXFLAGS) -c $< -o $@ && echo "  CXX\t $(@F)"

.PHONY: all re clean fclean help debug clean_comment modules

.SILENT: all re clean fclean help debug clean_comment $(NAME) $(OBJS)
