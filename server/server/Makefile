# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: wismith <wismith@42ABUDHABI.AE>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/24 10:00:02 by wismith           #+#    #+#              #
#    Updated: 2023/07/24 10:00:02 by wismith          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.SILENT:

NAME = server

SERVDIR = ftServerUtils
SERVARCH = $(SERVDIR)/ServerUtils.a

#* @note : no need to include .cpp extension in SRCS
#* 	@format : directory/file_name without .cpp
#*	@example : classDef/rawData

SRCS = main \
	client \
	server \
	network \
	parser \
	channels \
	welcome \
	commands/cinterface \
	commands/dcc \
	commands/quit \
	commands/cap \
	commands/join \
	commands/kick \
	commands/nick \
	commands/pass \
	commands/ping \
	commands/user \
	commands/privmsg \
	commands/mode \
	commands/topic \
	commands/invite \
	commands/part \
	commands/list \

OS:= ${shell uname}

ifeq ($(shell uname -s), Darwin)
	def = -D OSX
endif
ifeq ($(shell uname -s), Linux)
	def = -D LINUX
endif

CXX = g++

CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -g3

OBJDIR = object

OBJS = $(addprefix $(OBJDIR)/, $(addsuffix .o, $(SRCS)))

#* @note : to include a new directory within srcs
#* 	please add @mkdir -p $(OBJDIR)/directory_name to below
#*	@example : @mkdir -p $(OBJDIR)/classDef
$(OBJDIR)/%.o : srcs/%.cpp
	@mkdir -p $(OBJDIR)
	@mkdir -p $(OBJDIR)/commands
	$(CXX) $(CXXFLAGS) $(def) -c $< -o $@

all : $(NAME)

$(NAME): utils $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(SERVARCH) $(def) -o $(NAME)

utils :
	@make -s -C $(SERVDIR)

clean:
	@rm -rf $(OBJDIR)
	@make clean -s -C $(SERVDIR)

fclean: clean
	@rm -rf $(NAME)
	@make fclean -s -C $(SERVDIR)
	@rm -rf server.log

re: fclean all

#* @brief vars randomize the clients by date
LOGFILE = $(shell date)
CLIENTTIME = $(addsuffix .$(LOGFILE), client)
CLIENT = $(shell echo $(CLIENTTIME) | tr A-Z a-z | tr -d ' ' | tr -d ':' | tr -d '+' | tr -d '-')
CLIENT_VERSION = irssi:1.2.3

#* @client
#* @brief : make client will build and execute a client
#*	@note : quit client by typing "/quit" on interface
#* ipconfig getifaddr en0
#*
client:
	docker run -it --name $(CLIENT) -e TERM -u $(id -u):$(id -g) \
	--log-driver=none \
	   -v ${HOME}/.irssi:/home/user/.irssi:ro \
	$(CLIENT_VERSION)
	docker rm -f $(CLIENT)

#* @brief : make rm_client will remove the client and delete the image
rm_client:
	docker rmi $(CLIENT_VERSION)

.PHONY : all clean fclean re printStart printnl printProvided