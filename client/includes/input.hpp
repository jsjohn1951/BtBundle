#ifndef INPUT_HPP
#define INPUT_HPP

#include <string>
#include <unistd.h>
#include <iostream>

class input
{
	private :
		std::string			str;
		int					fd;

	public :
							input ();
							~input ();

		//* getters
		const std::string	&getStr() const;

		//* other
		std::string			get_next_line();
		void				receive();
};

#endif