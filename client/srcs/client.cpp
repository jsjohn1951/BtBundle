#include "../includes/client.hpp"

bool	g_client_run = true;

client::client () : input (), user("NewUser"), pars() {}

client::~client ()
{
	close (this->fd);
}

void	client::init(int port, const std::string ip)
{
	struct sockaddr_in serv_addr;

    if ((this->fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		std::cerr << "Socket failure!" << std::endl;
		exit (1);
	}

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, ip.c_str(), &serv_addr.sin_addr) <= 0)
	{
		std::cerr << "inet_pton failure!" << std::endl;
		exit (1);
	}

    if (connect(this->fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
	{
		std::cerr << "Connection failure!" << std::endl;
		exit (1);
	}

	this->pfd = NPOLL(this->fd);
	this->catch_signals();
}

void	client::Connect(std::string pass)
{
	std::string	cmd;

	cmd += "CAP LS\r\n";
	cmd += "PASS " + pass + "\r\n";

	cmd += "NICK "
		+ this->user + "\r\n";

	cmd += "USER "
		+ this->user
		+ " "
		+ this->user
		+ " localhost :ircClient\r\n";

	cmd += "CAP END\r\n";
	cmd += "JOIN #" + this->user + "\r\n";
	cmd += "INVITE bot #"
			+ this->user + "\r\n";

	poll(&this->pfd, 1, -1);
	if (this->pfd.revents & POLLOUT)
		send(this->fd, cmd.c_str(), cmd.size(), 0);
}

void	client::run()
{
	std::string	cmd = "QUIT :"
		+ this->user
		+ " is leaving!\r\n";
	fcntl(this->fd, F_SETFL, O_NONBLOCK);
	fcntl(1, F_SETFL, O_NONBLOCK);
	while (g_client_run)
	{
		poll(&this->pfd, 1, -1);
		input.receive();
		if (this->toLower(input.getStr()) == "quit")
			g_client_run = false;

		if (this->pfd.revents & POLLOUT)
		{
			std::string	str = "PRIVMSG #"
				+ this->user
				+ " "
				+ ":"
				+ input.getStr()
				+ "\r\n";
			if (input.getStr().size())
				send(this->fd, str.c_str(), str.size(), 0);
		}

		usleep (12000);

		if (this->pfd.revents & POLLIN)
		{
			pars.pRecv( Read(this->fd) );
			for (size_t j = 0; j < pars.getCmds().size(); j++)
				if (pars.getCmdSec(j)[0].size())
					pars.printCmds(pars.getCmdSec(j));
		}
		pars.clear();
	}
	poll(&this->pfd, 1, -1);
	if (this->pfd.revents & POLLOUT)
		send(this->fd, cmd.c_str(), cmd.size(), 0);
	close (this->fd);
}

std::string	client::Read(int fd)
{
	char	buff[513];
	std::string	res;
	ssize_t bits = recv(fd, buff, 512, 0);
	if (!bits || bits > 512)
		return (std::string());

	buff[bits] = '\0';
	std::string	str(buff);
	if (bits > 0)
	{
		if (str.find('\n') == str.npos && str.find('\r') == str.npos)
			this->buffer += str;
		else
		{
			res = this->buffer + str;
			this->buffer.clear();
			return (res);
		}
	}
	return (std::string());
}

std::string	client::toLower(const std::string str)
{
	char	nstr[str.size()];
	size_t	i = 0;

	for (; i < str.size(); i++)
		nstr[i] = tolower(str.c_str()[i]);
	nstr[i] = '\0';
	std::string res(nstr);
	return (res);
}

void	client::setUser(const std::string &U)
{
	this->user = U;
}

//! ------- SIGNAL HANDLING --------- !//

void	sighandlr(int signum)
{
	(void) signum;
	g_client_run = false;
	std::cout << "\nStopping client!" << std::endl;
}

void	client::catch_signals()
{
	signal(SIGINT, sighandlr);
	signal(SIGQUIT, sighandlr);
	signal(SIGHUP, sighandlr);
	signal(SIGTERM, sighandlr);
	signal(SIGPIPE, sighandlr);
}
