#! /bin/bash

printf " _______________________________________________________\n"
printf "|                                                       |\n"
printf "|";

tput setaf 2
printf "     \"THE MOMENT WE STOP LEARNING IS THE MOMENT WE     "
tput init

printf "|\n"
printf "|";

tput setaf 2
printf "    STAGNATE, AND THOUGH LEARNING IN THE REAL WORLD    "
tput init

printf "|\n"
printf "|"

tput setaf 2
printf "     IS SELDOM UNACCOMPANIED BY HARDSHIP IT'S ALSO     "
tput init

printf "|\n"
printf "|"

tput setaf 2
printf "          AN EXPERIENCE.\" - Willem J. Smith            "
tput init

printf "|\n"
printf "|_______________________________________________________|\n"
printf "\n\n"

printf "Is Docker installed? (y/n) "
read dInstall
printf "\n"

if [ "$dInstall" = "y" ];
	then
		if [ "$( docker ps 2> /dev/null | wc -l | tr -d ' ' )" = "0" ];
		then
			tput setaf 1
			echo "docker not running"
			tput init
			open -a Docker
			printf "Starting docker "
			while [ "$( docker ps 2> /dev/null | wc -l | tr -d ' ' )" = "0" ]
			do
				tput setaf 1
				printf " . "
				tput init
				sleep 2
			done
		fi
		printf "docker started!\n"
		docker-compose -f ./docker-compose.yml up -d --build
		./scripts/clientInit.sh
else
	tput setaf 1
	echo "Ouch, script intended for docker!"
	tput init

	echo "You are forced to launch each piece Manually!"
	echo "Check the README.md"
	echo
	echo "Space is the final fronteer, good luck 42 Cadet!"
fi
