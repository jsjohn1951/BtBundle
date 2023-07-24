/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wismith <wismith@42ABUDHABI.AE>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 19:41:07 by wismith           #+#    #+#             */
/*   Updated: 2023/05/31 21:08:48 by wismith          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/channel.hpp"

using namespace ft;

channel::channel ( const int &f, const std::string &n ) :
	name(n), backlog( ), fd( f ) { }

channel::~channel ( ) { }

void	channel::Send()
{
	std::string	msg;

	for (; this->backlog.size();)
	{
		msg += this->backlog.front();
		this->backlog.erase(this->backlog.begin());
	}

	send(this->fd, msg.c_str(), msg.size(), 0);
}

void	channel::addPrivMsg(std::string str)
{
	this->backlog.push_back("PRIVMSG "
			+ this->name + " :\x1B[32m"
			+ str + "\x1B[0m\n");
}

void	channel::addCmd(std::string str)
{
	this->backlog.push_back(str);
}

void	channel::addMsg(std::vector<std::string> &msg)
{
	std::vector<std::string>::iterator	it;

	it = msg.begin();
	for ( ; it != msg.end(); it++ )
		this->addPrivMsg(*it);
}

std::vector<std::string>	&channel::retrieveBacklog()
{
	return (this->backlog);
}
