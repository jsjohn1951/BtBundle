#! /bin/bash

clear
printf "Running client locally? "
tput setaf 1
printf "(y/n) "
tput init
read lclient
printf "\n"

if [ "${lclient}" = "y" ];
then
	make re -C ./client
	tput setaf 2
	printf "Attempting to init client...\n"
	tput init

	printf "This may take a while.\n"
	printf "Waiting for server "
	while true;
	do
		if [ -f server/server/server.log ];
		then
			tput setaf 2
			printf " done! "
			tput init
			sleep 1
			clear
			tput setaf 2
			printf "Server found!\n"
			tput init
			./client/client 127.0.0.1 6667 wismith bob
			make fclean
			make fclean -C client
			exit 0
		else
			tput setaf 2
			printf " . "
			tput init
			sleep 1
		fi
	done
else
	clear
	printf "Nothing to see here folks!\n\n"
	printf "You must run the client manually from remote machine.\n"
	printf "For Remote Client :\n"
	printf "\tUse host's IPv4 -> \""
	tput setaf 2
	ipconfig getifaddr en0 | tr -d '\n'
	tput init
	printf "\"\n\tUse host's Port -> \""
	tput setaf 1
	printf "6667"
	tput init
	printf "\"\n"
	printf "\tUse host's Password -> \""
	tput setaf 5
	printf "bob"
	tput init
	printf "\"\n"
fi
