NAME		=	btc
INC			= 	BitcoinExchange.hpp
CXX			=	c++
CXXFLAGS	=	-Wall -Wextra -Werror -std=c++98


AR		=	ar rc
RM		=	rm -f

SRC		=	main.cpp BitcoinExchange.cpp

OBJS	=	$(SRC:.cpp=.o)

.PHONY:		all clean fclean re

all:		$(NAME)

$(NAME): 	$(OBJS)
			$(CXX) $(OBJS) -o $(NAME)

%.o: %.cpp 	$(INC)
			$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
			$(RM) $(OBJS)

fclean: 	clean
			$(RM) $(NAME)

re:			clean all
