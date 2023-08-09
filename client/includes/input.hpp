#ifndef INPUT_HPP
#define INPUT_HPP

#include <string>
#include <unistd.h>
#include <iostream>

class Input
{
	private :
		std::string			str;
		int					fd;

	public :
							Input ();
							~Input ();

		//* getters
		const std::string	&getStr() const;

		//* other
		std::string			get_next_line();
		void				receive();
};

#endif