# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: wismith <wismith@42ABUDHABI.AE>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/09 21:45:59 by wismith           #+#    #+#              #
#    Updated: 2023/07/25 14:27:03 by wismith          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=client

UTILSDIR = ftServerUtils
UTILS = $(UTILSDIR)/ServerUtils.a

SRCS = main \
	input \
	client \
	parser

CXX = c++

CPPFLAGS = -Wall -Werror -Wextra -g3

OBJDIR = objects

OBJ = $(addprefix $(OBJDIR)/, $(addsuffix .o, $(SRCS)))

$(OBJDIR)/%.o : srcs/%.cpp
	@mkdir -p $(OBJDIR)
	@printf "\033[A\033[2K\r"

	@tput setaf 2
	@printf "\tGenerating ... "
	@tput init

	$(CXX) $(CPPFLAGS) -c $< -o $@

all : $(NAME)

$(NAME) : utility $(OBJ)
	@printf "\t"
	$(CXX) $(CPPFLAGS) $(OBJ) $(UTILS) -o $(NAME)
	@printf "\n"

utility :
	@make -C $(UTILSDIR)

clean :
	rm -rf $(OBJDIR)
	@make clean -C $(UTILSDIR)

fclean : clean
	rm -rf $(NAME)
	@make fclean -C $(UTILSDIR)

re : fclean all

.PHONY : all clean fclean re