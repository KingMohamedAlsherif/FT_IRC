# Makefile for ft_irc project
# NAME is the name of our executable
NAME = ircserv

# CXX is the compiler we use (c++)
CXX = c++

# CXXFLAGS are the compilation flags
# -Wall: enables all common warnings
# -Wextra: enables extra warnings
# -Werror: treats warnings as errors
# -std=c++98: ensures we use C++98 standard
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

# Source files - all .cpp files in our project
SRCS = main.cpp \
       Server.cpp \
       Client.cpp \
       Channel.cpp \
       Parser.cpp \
       Utils.cpp

# Object files - .cpp files converted to .o files
OBJS = $(SRCS:.cpp=.o)

# Header files - for dependency checking
HEADERS = Server.hpp \
          Client.hpp \
          Channel.hpp \
          Parser.hpp \
          Utils.hpp

# Default rule - builds the program
all: $(NAME)

# Rule to build the executable
# $@ means the target ($(NAME))
# $^ means all prerequisites ($(OBJS))
$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Rule to build object files from source files
# $< means the first prerequisite (the .cpp file)
# $@ means the target (the .o file)
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# clean rule - removes object files
clean:
	rm -f $(OBJS)

# fclean rule - removes object files and executable
fclean: clean
	rm -f $(NAME)

# re rule - rebuilds everything from scratch
re: fclean all

# .PHONY tells make that these aren't file names
.PHONY: all clean fclean re
