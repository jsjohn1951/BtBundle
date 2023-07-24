# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: wismith <wismith@42ABUDHABI.AE>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/27 09:28:30 by wismith           #+#    #+#              #
#    Updated: 2023/07/24 16:07:24 by wismith          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.SILENT:

NAME = bot

UTILSDIR = ftServerUtils
UTILS = $(UTILSDIR)/ServerUtils.a

SRCS = main \
	bot \
	parser \
	bot_commands \
	parse_config \
	channel \

OBJDIR = objects

OBJ = $(addprefix $(OBJDIR)/, $(addsuffix .o, $(SRCS)))

CXX = c++

CPPFLAGS = -Wall -Werror -Wextra -std=c++98 -g3

$(OBJDIR)/%.o : srcs/%.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CPPFLAGS) -c $< -o $@

all : $(NAME)

$(NAME) : utility $(OBJ)
	$(CXX) $(CPPFLAGS) $(OBJ) $(UTILS) -o $(NAME)

utility :
	@make -s -C $(UTILSDIR)

clean :
	@make clean -s -C $(UTILSDIR)
	rm -rf $(OBJDIR)

fclean : clean
	@make fclean -s -C $(UTILSDIR)
	rm -rf $(NAME)
	rm -rf bot.log

re : fclean all

.PHONY : all clean fclean re
	