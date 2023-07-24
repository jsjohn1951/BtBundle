#include "../includes/input.hpp"

input::input (): str(), fd(1)
{}

input::~input ()
{}

const std::string	&input::getStr() const
{
	return (this->str);
}

std::string	input::get_next_line()
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

void	input::receive()
{
	this->str = this->get_next_line();
}
