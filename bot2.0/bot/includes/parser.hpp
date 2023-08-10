#   ifndef PARSER_HPP
# define PARSER_HPP

#include <iostream>
#include <exception>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <poll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

#include "config.hpp"

#ifndef MSG
	# define MSG "bot.msg"
#endif

#ifndef CONF
	#define CONF "bot.conf"
#endif

# define LIMITS 0
# define BREAKS 1
# define NAMES 2

/* index of the config */
# define CONFN this->config[i]
# define CONF_FOUND(x) CONFN.find(x)
# define CONF_FIND(i, x) CONF_FOUND(x) == this->config[i].npos

typedef struct s_ret
{
	bool	limits = false, breaks = false, names = false;
}   t_ret;

typedef struct s_specs
{
	bool	limits = false, breaks = false, names = false;
	bool	start = false;
}   t_specs;

# define NPOLL(nfd) (struct pollfd){ .fd = nfd, .events = POLLIN | POLLOUT, .revents = 0 }

class parser : public config
{
	protected :
	struct pollfd	   pfds;
	int	 sock;
	size_t	  port;

	std::vector<std::string>	msg;
	// std::string	 msg;
	std::vector<std::string>	config;
	std::string	 pass;

	std::vector<std::string>		cmds;
	std::vector<std::string>	cmd_sec;
	std::string	 buffer;

	public :
	parser ();
	~parser ();

	int	 getSock () const;
	size_t	  getPort () const;

	void	args ( int argc, char **argv );
	void	retrieveMsg ();
	void	retrieveConfig ();
	void	eraseComments ();

	/** Parse config */
	void	names(t_ret &ret, std::string &line);
	void	Overlord( const std::string &line, t_param &param);
	void	limitDis();

	void	printMsg ();
	void	printConf ();
	void	printUsers ();
	std::pair<std::string, std::string>	ft_split(std::string &str, const std::string &delims);

	void	pRecv (const std::string &cmd);
	void		Read();
	std::vector<std::string>			&getCmdSec(size_t i);
};

#endif