#   ifndef CONFIG_HPP
# define CONFIG_HPP

# define LIMITS 0
# define BREAKS 1
# define NAMES 2

#include <map>
#include <string>
#include <chrono>
#include <ctime>

# define MALE 0
# define FEMALE 1

# define SEATED 0
# define BATHROOM 1
# define PRAYER 2
# define EMERGENCY 3
# define LUNCH 4

typedef struct  s_param
{
	bool	pr;
	bool	bt;

	int pray;
	int bth;
}   t_param;

typedef struct  s_subj
{
	std::string name;
	int	 		gen;
	int	 		status;
	t_param	 	breaks;
	std::time_t	elapsed;
}   t_subj;

class config
{
	protected :
	int	 MODE;
	t_param	limits;
	t_param	breaks;
	std::map<std::string, t_subj>   sub;

	public :
	config ();
	~config ();

	const int	   &getMode() const;
	const t_param	   &getLimits() const;
	const t_param	   &getBreaks () const;
	std::map<std::string, t_subj>   &getSub ();
};

std::string	&operator<<(std::string &o, t_subj &sub);

#endif