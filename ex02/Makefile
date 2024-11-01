NAME		=	PmergeMe
INC			= 	PmergeMe.hpp
CXX			=	c++
CXXFLAGS	=	-Wall -Wextra -Werror -std=c++98 -g

AR		=	ar rc
RM		=	rm -f

SRC		=	main.cpp PmergeMe.cpp

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
