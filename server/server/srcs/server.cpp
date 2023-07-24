/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wismith <wismith@42ABUDHABI.AE>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 13:45:38 by wismith           #+#    #+#             */
/*   Updated: 2023/05/11 18:15:29 by wismith          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server.hpp"

using namespace ft;

bool	ft::g_server_run = RUNNING;

/** @brief Port and Password Constructor
 * 		initializes values to default settings
*/
server::server(int nport, std::string pw) : network(pw), state(SHUTDOWN), opt(1),
				port(nport), lstn() {}

/** @brief Destructor
 * @note closes any open file descriptors
*/
server::~server()
{
	this->log << "Closing client fds\n";
	for (size_t i = 0; i < this->pfds.size(); i++)
		(i ? close (this->pfds[i].fd) : (int) i);
	this->log << "Shutting Down Server";
}

/** @brief server init calls lstnInit to initialize the listener,
 * 			pushes a new struct pollfd with values initialized with the macro NPOLL,
 * 			if an exception hasn't been thrown yet, the state is set to RUNNING
*/
void	server::init()
{
	ft::catch_signals();
	this->log << "initializing the listener";
	this->lstnInit();
	this->pfds.push_back(NPOLL(this->lstn.getSock()));
	this->state = RUNNING;
}

/** @brief lstnInit initializes the listener with the port
 * 			Taken as param for constructor.
 * 			Inits sockaddr_in.
 * 			Sets the socket to non blocking.
 * 			Binds the socket to fd.
 * 			Listens with the socket.
*/
void	server::lstnInit()
{
	this->opt = 1;
	this->log << this->lstn.setInfo(AF_INET, SOCK_STREAM, IPPROTO_TCP, this->port, INADDR_ANY);
	this->log << this->lstn.setSockProto(SOL_SOCKET, SO_REUSEADDR, this->opt);
	this->log << this->lstn.setSockProto(SOL_SOCKET, SO_REUSEPORT, this->opt);
	this->log << this->lstn.nonBlocking();
	this->log << this->lstn.BindConnect();
	this->log << this->lstn.ListenConnect();
	this->log << " ";
}

/** @brief registers a new  client
 * 	@note new client is stored within a std::map
 * 			with key values being the file descriptor returned
 * 			by accept.
 * 		Each client is associated with a pollfd structure stored
 * 			within a vector.
*/
void	server::regNewClient()
{
	std::string	fdStr;
    int 		fd = -1;
    int 		addrlen = 0;
    struct 		sockaddr_in address;
    memset(&address, 0, sizeof(address));

	this->log << "[ Client Attempting to Connect ]";
    if ((fd = accept(this->lstn.getSock(), (sockaddr *)&address, (socklen_t *)&addrlen)) < 0
		|| this->clients.size() >= 100)
		this->log << "Client unable to connect!\n";
	else
	{
		this->clients[fd] = client(fd);
		this->pfds.push_back(NPOLL(fd));
		this->log << "Accepting new client, fd : " + (fdStr << fd);
	}
	this->log << " ";
}

/** @brief function to receive commands coming from the client,
 * 			commands are then parsed and can be retrieved by the
 * 			getCmdSec(size_t); method which returns the indexes of
 * 			a vector of strings.
 * 			The selCmd(vector<std::string> &, int ) method is used to
 * 			select the appropriate command and to execute it.
*/
void	server::receiveCmds(size_t i)
{
	std::string	cmd;
	size_t		pos = cmd.npos;

	cmd << M_CLIENT(i);
	this->pRecv(cmd);

	while ((pos = cmd.find('\n', pos + 1)) != cmd.npos)
		cmd[pos] = ' ';
	while ((pos = cmd.find('\r', pos + 1)) != cmd.npos)
		cmd[pos] = ' ';
	this->log << " << " + cmd;

	for (size_t j = 0; j < this->getCmds().size(); j++)
		if (this->getCmdSec(j)[0].size())
			this->selCmd(this->getCmdSec(j), i);
}

/** @brief sendReply is used to send and unload the backlog for each
 * 			client. The backlog contains commands that trigger events
 * 			within the client ( ref client: irssi ), e.g: joining a channel.
*/
void	server::sendReply(size_t i)
{
	std::string	reply;
	std::string	rep;
	size_t		pos;
	client	&client = M_CLIENT(i);

	while (client.getBacklogSize())
	{
		rep = client.retrBacklog();
		reply += rep;


		while ((pos = rep.find('\n', pos + 1)) != rep.npos)
			rep[pos] = ' ';
		while ((pos = rep.find('\r', pos + 1)) != rep.npos)
			rep[pos] = ' ';
		this->log << " >> " + rep;


	}
	reply >> client;
}

/** @brief rmClient method is to remove
 * 			a client from the system and close it's
 * 			file descriptor.
*/
bool	server::rmClient(size_t i)
{
	close (pfds[i].fd);
	this->clients.erase(pfds[i].fd);
	this->pfds.erase(pfds.begin() + i);
	return (true);
}

/** @brief run method contains the main server loop */
void	server::run()
{
	std::string	cmd;
	bool		clientRM = false;

	std::cout << "Server Running ..." << std::endl;

	/** @brief main server loop, will run until a specific
	 * 			signal is received, or the state changes.
	*/
	while (ft::g_server_run && this->state)
	{
		/** @brief checks the array of fds in the vector to see
		 * 			if they are ready to read or write to.
		*/
		poll(this->pfds.data(), this->pfds.size(), -1);

		/** @brief 0'th index of pfds is the master socket/fd.
		 * 			Checking POLLIN of the master fd establishes
		 * 			whether a client is trying to connect, and
		 * 			registers new client with regNewClient;
		*/
		if (this->pfds[0].revents & POLLIN)
				this->regNewClient();

		/** @brief loops through each index of the pfds vector
		 * 			checking for POLLIN and POLLOUT of each client
		*/
		for (size_t i = 1; i < this->pfds.size(); i++)
		{
			/** @brief finds the right client */
			client	&client = M_CLIENT(i);

			/** @brief checks whether client is ready to write
			 * 			to using the following :
			 * 				this->pfds[i].revents & POLLOUT
			*/
			if (this->pfds[i].revents & POLLOUT)
				this->sendReply(i);

			/** @brief sending welcome message when
			 * 			a client gets registered.
			 * 			The welcome class is similar to a command
			 * 				in that it inherits from cinterface, however
			 * 				it is not a command.
			*/
			if ( !client.getReg().welcomeSent
					&& client.is_registered() )
				client.getReg().welcomeSent
					= welcome( this->clients,
									this->pfds,
									this->password ).Welcome( client );

			/** @brief a client that requests /quit command is
			 * 			marked for deletion in the quit command.
			 * 			The removal happens when a call to rmClient
			 * 			method is made with index to client.
			*/
			if (client.getIsMarkForDel())
			{
				clientRM = this->rmClient(i);
				break ;
			}

			/** @brief section to receive command/string from
			 * 			client fd.
			 * 			If the following evaluates to true that
			 * 			signifies the client is ready to read from :
			 * 				this->pfds[i].revents & POLLIN
			*/
			if (!clientRM && this->pfds[i].revents & POLLIN)
				this->receiveCmds(i);

			clientRM = false;

			/** @brief clears the cmds vector of the command received
			 * 			and it's arguments.
			*/
			this->clear();
		}
	}
}

/** @brief Non-members */

/** @brief signal handler stops the server upon receiving
 * 			a signal regardless of the signal.
*/
void	sighandlr(int signum)
{
	(void) signum;
	ft::g_server_run = SHUTDOWN;
	std::cout << "\nStopping server!" << std::endl;
}

/** @brief catch_signal function is used to catch the specific
 * 			signals we server handles. This reduces the possibility
 * 			of memory leaks during the runtime, and provides the user
 * 			a way of shutting down the server.
 * 
 * 			Signals handled :
 * 
 * 				SIGINT
 * 				SIGQUIT
 * 				SIGHUP
 * 				SIGTERM
 * 				SIGPIPE
*/
void	ft::catch_signals()
{
	signal(SIGINT, sighandlr);
	signal(SIGQUIT, sighandlr);
	signal(SIGHUP, sighandlr);
	signal(SIGTERM, sighandlr);
	signal(SIGPIPE, sighandlr);
}
