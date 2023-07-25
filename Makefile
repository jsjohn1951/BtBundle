# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: wismith <wismith@42ABUDHABI.AE>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/24 17:24:39 by wismith           #+#    #+#              #
#    Updated: 2023/07/25 14:14:33 by wismith          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = EXAMBOT

SRCS = ./docker-compose.yml
COMP = docker-compose
DIRFLAG =  -f $(SRCS)

LOGS = ./log
SERVERLOG = server/server/server.log
BOTLOG = bot/bot/bot.log

BOTCONF = conf/bot.conf

all : $(NAME)

$(NAME) : mvBotConf
	@./scripts/dockerInit.sh

down : docDown mvServer cpBot

docDown :
	$(COMP) $(DIRFLAG) down --rmi all

mvBotConf : $(BOTCONF)
	cp $(BOTCONF) bot/bot/

mvServer : $(SERVERLOG)
	mkdir -p $(LOGS)
	rm -rf $(SERVERLOG)

cpBot : $(BOTLOG)
	mkdir -p $(LOGS)
	mv ./$(BOTLOG) $(LOGS)

bot.conf : $(BOTCONF)
	@tput setaf 2
	@echo "Configuration File Provided!"
	@tput init

$(BOTCONF) :
	@mkdir -p conf
	@echo "#* BOT BREAKS contains the break settings (e.g: PRAR 4)" > $(BOTCONF)
	@echo "#* 'bot.conf' is indentation sensitive. Tabs should not be used." >> $(BOTCONF)
	@echo "#* Use spaces instead, unless writing inside a comment denoted by '#'" >> $(BOTCONF)
	@echo >> $(BOTCONF)

	@echo "#* Sections :" >> $(BOTCONF)
	@echo "#*	1. BOT LIMITS" >> $(BOTCONF)
	@echo "#*	2. BOT BREAKS" >> $(BOTCONF)
	@echo "#*	3. BOT NAMES" >> $(BOTCONF)
	@echo >> $(BOTCONF)
	
	@echo "#! ----------------< Number of pisciners allowed in each room >------- #" >> $(BOTCONF)
	@echo >> $(BOTCONF)
	@echo BOT LIMITS >> $(BOTCONF)
	@echo END >> $(BOTCONF)

	@echo >> $(BOTCONF)

	@echo "#! ----------------< Number of breaks per pisciner >------------------ #" >> $(BOTCONF)
	@echo >> $(BOTCONF)
	@echo BOT BREAKS >> $(BOTCONF)
	@echo PRAR 0 >> $(BOTCONF)
	@echo BTH 2 >> $(BOTCONF)
	@echo END >> $(BOTCONF)

	@echo >> $(BOTCONF)

	@echo "#! ----------------< List of pisciners including genders >------------ #" >> $(BOTCONF)
	@echo >> $(BOTCONF)
	@echo BOT NAMES >> $(BOTCONF)
	@echo "User1 M" >> $(BOTCONF)
	@echo "User2 F" >> $(BOTCONF)
	@echo END >> $(BOTCONF)

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