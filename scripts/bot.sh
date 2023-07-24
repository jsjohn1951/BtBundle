#! /bin/bash
x=1
cd /bot
make re
while [ $x = 1 ];
	do
	if [ -f /server/server.log ];
		then
			valgrind --leak-check=full ./bot 127.0.0.1 6667 bob
	else
		printf "server not started!\n"
		sleep 1
	fi
done