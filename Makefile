# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: wismith <wismith@42ABUDHABI.AE>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/24 17:24:39 by wismith           #+#    #+#              #
#    Updated: 2023/07/24 19:42:21 by wismith          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = EXAMBOT

SRCS = ./docker-compose.yml
COMP = docker-compose
DIRFLAG =  -f $(SRCS)

LOGS = ./log
SERVERLOG = server/server/server.log
BOTLOG = bot/bot/bot.log


all : $(NAME)

$(NAME) : up
	@clear
	@./scripts/clientInit.sh

up :
	$(COMP) $(DIRFLAG) up -d --build

down : docDown mvServer cpBot

docDown :
	$(COMP) $(DIRFLAG) down --rmi all

mvServer : $(SERVERLOG)
	mkdir -p $(LOGS)
	rm -rf $(SERVERLOG)

cpBot : $(BOTLOG)
	mkdir -p $(LOGS)
	mv ./$(BOTLOG) $(LOGS)

$(SERVERLOG) :
	touch $(SERVERLOG)

$(BOTLOG) :
	touch $(BOTLOG)

prune :
	@tput setaf 1
	docker builder prune -f
	@tput init

fclean : down prune
	make fclean -C server/server
	make fclean -C bot/bot

rmi :
	docker rmi all

re : fclean all

.PHONY : all up down prune fclean rmi re