#include "../../includes/log.hpp"

log::log (const std::string &name) : fname(name), outfile() {
	time_t		now = time( 0 );
	tm			*gmtm = gmtime( &now );
	std::string time( asctime( gmtm ) );

	this->outfile.open(this->fname.c_str(), std::ofstream::out | std::ofstream::app);
	this->outfile << "Log Start: " << time << std::endl;
	this->outfile.close();
}

log::~log () {
	this->outfile.open(this->fname.c_str(), std::ofstream::out | std::ofstream::app);
	if (this->outfile.good())
	{
		this->outfile << std::endl;
		this->outfile.close();
	}
}

void			log::parsLog(const std::string &msg)
{
	std::string	str = msg;
	size_t	pos = -1;

	while ((pos = str.find('\r', pos + 1)) != str.npos)
		str.erase(pos, 1);

	std::stringstream			ss(str);
	std::string					line;

	pos = -1;
	while (!ss.eof())
	{
		getline(ss, line, '\n');
		this->nextLog.push_back(line);
	}
}

void	log::pushLog(const std::string &msg)
{
	time_t		now = time( 0 );
	tm			*gmtm = gmtime( &now );
	std::string time( asctime( gmtm ) );
    std::string Content;

	time.erase(time.find('\n'));
	this->parsLog(msg);

	this->outfile.open(this->fname.c_str(), std::ofstream::out | std::ofstream::app);
	while (this->nextLog.size())
	{
		Content += this->nextLog.front();
		Content += "\ttime < " + time + " > ";
		this->nextLog.pop_front();
	}
	if (this->outfile.good())
	{
		this->outfile << Content;
		this->outfile << "\n";
		outfile.close();
	}
}

log	&operator<<(log &l, std::string const &msg)
{
	l.pushLog(msg);
	return (l);
}
