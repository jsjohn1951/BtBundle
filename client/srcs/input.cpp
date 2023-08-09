#include "../includes/input.hpp"

Input::Input (): str(), fd(1)
{}

Input::~Input ()
{}

const std::string	&Input::getStr() const
{
	return (this->str);
}

std::string	Input::get_next_line()
{
	std::string			line;
	char				buff[2];
	int					err = 1;

	while ( err > 0 && line.find('\n') == line.npos )
		if ( (err = read ( this->fd, buff, 1 )) >= 0 )
		{
			buff[err] = '\0';
			line += std::string (buff);
		}

	if (err == 0)
		return (std::string());
	if (*(line.end() - 1) == '\n')
		return ( line.substr(0, line.length() - 1) );
	return ( line );
}

void	Input::receive()
{
	this->str = this->get_next_line();
}
