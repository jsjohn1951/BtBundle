#include "../../includes/log.hpp"

logger::logger () : fname(""), outfile() {}

logger::logger (const std::string &name) : fname(name), outfile() {
	std::chrono::system_clock::time_point tm = std::chrono::system_clock::now();
	std::time_t		current = std::chrono::system_clock::to_time_t(tm);

	this->outfile.open(this->fname.c_str(), std::ofstream::out | std::ofstream::app);
	this->outfile << "Log Start: " << std::ctime(&current) << std::endl;
	this->outfile.close();
}

logger::~logger () {
	this->outfile.open(this->fname.c_str(), std::ofstream::out | std::ofstream::app);
	if (this->outfile.good())
	{
		this->outfile << std::endl;
		this->outfile.close();
	}
}

void			logger::parsLog(const std::string &msg)
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

void	logger::pushLog(const std::string &msg)
{
	std::chrono::system_clock::time_point tm = std::chrono::system_clock::now();
	std::time_t		current = std::chrono::system_clock::to_time_t(tm);
	std::string 	time(std::ctime(&current));
    std::string Content;

	time.erase(time.find('\n'));
	this->parsLog(msg);

	this->outfile.open(this->fname.c_str(), std::ofstream::out | std::ofstream::app);
	while (this->nextLog.size())
	{
		Content += this->nextLog.front();
		Content += "\ttimestamp < " + time + " > ";
		this->nextLog.pop_front();
	}
	if (this->outfile.good())
	{
		this->outfile << Content;
		this->outfile << "\n";
		outfile.close();
	}
}

std::string	logger::time_convert(time_t	&time)
{
	std::string timeElapsed;
	timeElapsed = ((time / (60 * 60)) < 10 ? "0" : "")
		+ std::to_string ((time / (60 * 60)))
		+ ":"
		+ (((time / 60) % 60) < 10 ? "0" : "")
		+ std::to_string (((time / 60) % 60))
		+ ":"
		+ ((time % 60) < 10 ? "0" : "")
		+ std::to_string (time % 60);
		return (timeElapsed);
}

logger &operator<<(logger &l, std::string const &msg)
{
	l.pushLog(msg);
	return (l);
}
