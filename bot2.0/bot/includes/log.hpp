#   ifndef LOG_HPP
# define LOG_HPP

#include <fstream>
#include <string>
#include <sstream>
#include <chrono>
#include <ctime>
#include <list>

class logger
{
	std::list<std::string>		nextLog;
	std::string					fname;
	std::ofstream				outfile;
	void						parsLog(const std::string &msg);

	public :
	logger ();
	logger (const std::string &);
	~logger ();

	std::string		time_convert(time_t	&time);
	//! file modifier
	void			pushLog(const std::string &msg);
};

logger &operator<<(logger &, const std::string &);

#endif