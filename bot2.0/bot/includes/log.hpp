#   ifndef LOG_HPP
# define LOG_HPP

#include <fstream>
#include <string>
#include <sstream>
// #include <time.h>
#include <chrono>
#include <ctime>  
#include <list>

class log
{
	std::list<std::string>	  nextLog;
	std::string	 fname;
	std::ofstream		outfile;
	void			parsLog(const std::string &msg);

	public :
	log (const std::string &);
	~log ();

	//! file modifier
	void			pushLog(const std::string &msg);
}; 

log &operator<<(log &, const std::string &);

#endif