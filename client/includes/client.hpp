#ifndef CLIENT_HPP
#define CLIENT_HPP

# include "../ftServerUtils/includes/ServerUtility.hpp"
# include "input.hpp"
# include "parser.hpp"
# include <iostream>
# include <string>
# include <poll.h>
# include <signal.h>

# define NPOLL(nfd) (struct pollfd){ .fd = nfd, .events = POLLIN | POLLOUT, .revents = 0 }

class client
{
	private :
		int				fd;
		struct pollfd	pfd;
		Input			input;
		std::string		user;
		std::string		buffer;
		ft::parser		pars;

	public :
						client ();
						~client ();

		//* other
		void			init (int , const std::string);
		void			Connect (std::string pass);
		void			run ();
		void			catch_signals();
		void			setUser(const std::string &U);
		std::string		Read(int fd);
		std::string		toLower(const std::string str);
};

#endif