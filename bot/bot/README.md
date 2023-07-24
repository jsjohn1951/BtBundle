# 42 Bt_IrcBot
## Bot created for use in ft_irc

The ft_irc bot was designed to keep track of breaktimes during an exam at a 42 campus. Each student/pisciner has a maximum number of times the bot allows them to take a break, afterwhich the bot will message no breaks available. 
All new evens are logged with a timestamp in a bot.log file. For example,
if a user enters the following :
``` > bot bth username ```
the bot will send a message back :
``` username status set : [ WENT BATHROOM ] ```
and will log the following inside of bot.log :
```` username : bathroom break time < Wed May 31 21:42:47 2023 > ````

## Installation
Requires that git is installed on your machine.
Requires an irssi client and irc server for user to interact with the bot. 
Once git is installed, use the following command to clone the repo.

```bash
git clone https://github.com/jsjohn1951/bt_ircbot
```
Once cloned enter the following commands in terminal to build the executable:
```bash
$> cd bt_ircbot
$> make
```
## Config
The bot's configuration can be set inside the bot's 'config.bot' file.
The key words : "BOT BREAKS" is to denote the start of the break time limits settings.
The key words : "BOT NAMES" is to denote the start of the username list to be taken into
the system.
Each denoted section will end with "BOT <section> END," see an example inside the config!
Lines starting with '#' are ignored by the parser, and can be used to include comments inside 'config.bot.'

## Usage
step 1. Set your bot configuration inside config.bot ( Read about the config above! )
step 2. Run your irc server
step 3. inside bt_ircbot directory run
``` ./bt_ircbot <IPv4> <port> <irc server password>```
example :
``` ./bt_ircbot 127.0.0.1 6667 pw ```
The bot will run infinitely, or will print an error if a systemcall failed or was unable to connect to server.
After starting the bot, it should join the server automatically as an automated client.
step 4. Run your irc client ( irssi recommended ), and connect to the server using the following commands:
``` /connect <IPv4> <port> <irc server password> ```
step 5. Now that both client and bot has joined to the server, enter the following commands to the client in order to create a new channel and invite the bot:
``` /join #chan```
``` /invite bot```
The bot will then automatically join the channel.
###### The Following are currently supported commands:
Each command must be preceeded with 'bot' keyword, and are not case sensitive.
example : ```bot Add user``` - will add a username 'user' to the bot

```
ADD    <username> - adding new username to keep track of
REMOVE <username> - removing username from the list
STATUS <username> - checks the username's status and number of breaks left
BTH    <username> - sets the status to bathroom break
PRAR   <username> - sets the status to prayer break
BACK   <username> - sets the status to seated
FINISH <username> - sets the status to finished exam
LIST              - lists all usernames on the system
HELP              - sends a list of available commands
```
###### Stop the bot by entering either ``` ctrl - c ``` or ``` ctrl - \ ```.