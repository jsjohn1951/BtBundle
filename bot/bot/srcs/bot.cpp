/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wismith <wismith@42ABUDHABI.AE>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 13:54:16 by wismith           #+#    #+#             */
/*   Updated: 2023/07/24 22:38:33 by wismith          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/bot.hpp"

using namespace ft;

s_subj::s_subj (int bth_q, int prar_q, int gen) : bth(bth_q), prar(prar_q), status(SEATED), gender(gen) {}

s_subj::~s_subj () {}

s_track_num::s_track_num () : mBth(0), mPrar(0), fBth(0), fPrar(0) {}

s_track_num::~s_track_num () {}

bool		g_bot_run = true;

bot::bot () : log("bot.log"), fd(0), pfd(), buffer(), pars(), msgs(), backlog(),
	chan(), track (), funcs (), m_mode(true), subj ()
{
	this->funcs["remove"] = &bot::remove;
	this->funcs["status"] = &bot::status;
	this->funcs["bth"] = &bot::bth;
	this->funcs["prar"] = &bot::prar;
	this->funcs["emg"] = &bot::emg;
	this->funcs["finish"] = &bot::finish;
	this->funcs["back"] = &bot::back;
	this->funcs["monitoring"] = &bot::monitoring_mode;
}

bot::~bot() {
	std::map<std::string, t_subj *>::iterator	it;
	MAPPY::iterator								mit;

	for (it = this->subj.begin(); it != this->subj.end(); it++)
		delete it->second;
	for (mit = this->channels.begin(); mit != this->channels.end(); mit++)
		delete mit->second;
	close (this->fd);
}

void	bot::init (int port, const std::string ip)
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

void	bot::Connect(std::string pass)
{
	std::string	cmd;

	cmd += "CAP LS\r\n";
	cmd += "PASS " + pass + "\r\n";
	cmd += "NICK bot\r\n";
	cmd += "USER bot bot localhost :ircbot\r\n";
	cmd += "CAP END\r\n";
	cmd += "JOIN #btBot\r\n";
	this->chan.push_back("#btBot");
	this->channels["#btBot"] = new ft::channel( this->fd, "#btBot" );

	poll(&this->pfd, 1, -1);
	if (this->pfd.revents & POLLOUT)
		send(this->fd, cmd.c_str(), cmd.size(), 0);
}

void	bot::run ()
{
	std::string quit = "QUIT :bot is leaving!\r\n";

	std::cout << "Bot Running!" << std::endl;

	while (g_bot_run)
	{
		poll(&this->pfd, 1, -1);


		if (this->pfd.revents & POLLOUT)
		{
			MAPPY::iterator	it;
			for ( it = this->channels.begin();
				it != this->channels.end(); it++)
			{
				std::vector<std::string>	&bckl = it->second->retrieveBacklog();
				if (!bckl.size())
					continue ;
				send(this->fd, bckl[0].c_str(), bckl[0].size(), 0);
				bckl.erase(bckl.begin());
			}
		}

		usleep (12000);

		if (this->pfd.revents & POLLIN)
		{
			pars.pRecv( Read(this->fd) );
			for (size_t j = 0; j < pars.getCmds().size(); j++)
			{
				std::vector<std::string> vec = pars.getCmdSec(j);
				if (vec[0].size())
					this->selCmd(vec);
			}
		}
		pars.clear();
	}
	if (this->pfd.revents & POLLOUT)
		send(this->fd, quit.c_str(), quit.size(), 0);
    close(this->fd);
}

std::string	bot::toLower(const std::string str)
{
	char	nstr[str.size()];
	size_t	i = 0;

	for (; i < str.size(); i++)
		nstr[i] = tolower(str.c_str()[i]);
	nstr[i] = '\0';
	std::string res(nstr);
	return (res);
}

void		bot::selCmd(std::vector<std::string> cmd)
{
	if (cmd.size() < 3)
		return ;

	if (cmd.size() >= 4 && cmd[1] == "PRIVMSG" && cmd[3] == ":bot")
	{
		if (this->toLower(cmd[4]) == "help")
			this->channels[cmd[2]]->addMsg(this->msgs);
		// else if (this->toLower(cmd[4]) == "list")
		// 	this->list(cmd[2]);
		else if (this->toLower(cmd[4]) == "num")
			this->num(cmd[2]);
		else if (cmd.size() >= 6)
		{
			this->backlog.push_back("");
			if (this->toLower(cmd[4]) == "add" && cmd.size() == 7)
				this->add(cmd[5], cmd[6], cmd[2]);
			else
				if (this->funcs.find(this->toLower(cmd[4])) != this->funcs.end())
					(this->*funcs[this->toLower(cmd[4])]) (cmd[5], cmd[2]);
			this->backlog.push_back("");
		}
	}
	else if (cmd[1] == "INVITE")
	{
		cmd[3].erase(cmd[3].begin());

		if (this->channels.find(cmd[3]) != this->channels.end())
			return ;
		this->channels["#btBot"]->addCmd("JOIN " + cmd[3] + "\r\n");
		this->channels[cmd[3]] = new ft::channel( this->fd, cmd[3] );
	}
	else if (cmd[1] == "KICK")
		this->channels["#btBot"]->addCmd("JOIN " + cmd[3] + "\r\n");
}

std::string	bot::Read(int fd)
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

void	bot::retrieveMsgs()
{
	std::ifstream	infile(MSGFILE.c_str());

	for (int i = 0; i < 2; i++)
		this->msgs.push_back(std::string(" -- "));
	if (infile.good())
		for (std::string line; getline(infile, line);)
			this->msgs.push_back(line);
	else
	{
		std::cout << "No file exists!\n";
		return ;
	}
	for (int i = 0; i < 2; i++)
		this->msgs.push_back(std::string(" -- "));
	infile.close();
}

//! ------- SIGNAL HANDLING --------- !//

void	sighandlr(int signum)
{
	(void) signum;
	g_bot_run = false;
	std::cout << "\nStopping bot!" << std::endl;
}

void	bot::catch_signals()
{
	signal(SIGINT, sighandlr);
	signal(SIGQUIT, sighandlr);
	signal(SIGHUP, sighandlr);
	signal(SIGTERM, sighandlr);
	signal(SIGPIPE, sighandlr);
}
