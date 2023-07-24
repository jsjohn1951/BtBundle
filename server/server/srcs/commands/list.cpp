#include "../../includes/commands.hpp"

using namespace ft;

list::list(std::map<CLIENT_FD, CLIENT> &c, std::vector<pollfd> &p, std::string &pw,
	std::map<std::string, ft::channels *>	&chans) :
	ft::cinterface(c, p, pw), chan(chans) {}

list::~list() {}

void	list::exec(int i_pfds, const std::vector<std::string> &cmds)
{
	ft::client	&client = M_CLIENT(i_pfds);
	std::map<std::string, ft::channels *>::iterator	it;
	std::string	chan_name;
	std::string	chan_topic;
	std::string	chan_users;
	std::string	chan_mode;

	if (cmds.size() <= 2)
	{
		this->reply(client, RPL_LISTSTART, "LIST of all Channels\r\n");
		for (it = this->chan.begin(); it != this->chan.end(); it++)
		{
			chan_name = it->second->getChannelName();
			chan_topic = it->second->get_topic();
			chan_users = it->second->get_users();
			this->reply(client, RPL_LIST, "\x03#" + chan_name + "\x03"
			+ "\nUsers: " + chan_users + "\nTopic: \"" + chan_topic + "\"\r\n");
		}
		this->reply(client, RPL_LISTEND, ":End of LIST");
	}
	else
	{
		chan_name = cmds[1];
		if (this->chan.find(chan_name) == this->chan.end())
		{
			this->reply(client, ERR_NOSUCHCHANNEL, chan_name);
			return ;
		}
		chan_topic = this->chan[chan_name]->get_topic();
		chan_users = this->chan[chan_name]->get_users();
		chan_mode = this->chan[chan_name]->get_mode();
		this->reply(client, RPL_LIST, chan_name + " " + chan_users + " :" + chan_topic);
		this->reply(client, RPL_LISTEND, ":End of LIST");
	}
}