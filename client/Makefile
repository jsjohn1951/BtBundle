# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: wismith <wismith@42ABUDHABI.AE>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/09 21:45:59 by wismith           #+#    #+#              #
#    Updated: 2023/07/21 17:07:13 by wismith          ###   ########.fr        #
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
	@clear

	@tput setaf 1
	@printf "_________ .____    .______________ __________________\n"                                          
	@printf "\\_   ___ \\|    |   |   \\_   _____/ \\      \\__    ___/\n"
	@printf "/    \\  \\/|    |   |   ||    __)_  /   |   \\|    |   \n"
	@printf "\\     \\___|    |___|   ||        \\/    |    \\    |   \n"
	@printf " \\______  /_______ \\___/_______  /\\____|__  /____|   \n"
	@printf "        \\/        \\/           \\/         \\/         \n\n"
	@tput init

	@printf "\tExecute with:\n\n\t\t"
	@printf "./client <"

	@tput setaf 1
	@printf "IPv4"
	@tput init
	
	@printf "> <"

	@tput setaf 1
	@printf "port"
	@tput init
	
	@printf "> ["

	@tput setaf 1
	@printf "(optional) chanName"
	@tput init

	@printf "] <"

	@tput setaf 1
	@printf "password"
	@tput init

	@printf ">\n"

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