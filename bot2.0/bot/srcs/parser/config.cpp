#include "../../includes/config.hpp"

config::config () : MODE (-1), limits((t_param) {false, false, 0, 0}),
	breaks((t_param) {false, false, 0, 0}), sub() {}

config::~config () {}

const int &config::getMode () const
{
    return (this->MODE);
}

const t_param &config::getLimits() const
{
    return (this->limits);
}

const t_param   &config::getBreaks() const
{
    return (this->breaks);
}

std::map<std::string, t_subj>   &config::getSub ()
{
    return (this->sub);
}

std::string    &operator<<(std::string &o, const std::string &str)
{
    o += str;
    return (o);
}

std::string    &operator<<(std::string &o, t_subj &sub)
{
    o << sub.name
        << std::string(" gender: ")
        << std::string ("[ ")
        << (sub.gen == FEMALE ? std::string("F") : std::string("M"))
        << std::string(" ] ")
        << std::string(" STATUS: ")
        << std::string ("[ ")
        << std::string (sub.status == SEATED ? "SEATED" :
            sub.status == BATHROOM ? "BATHROOM" :
            sub.status == PRAYER ? "PRAYER" :
            sub.status == EMERGENCY ? "EMERGENCY" :
            sub.status == LUNCH ? "LUNCH" :
            "WAREABOUTS UNKNOWN")
        << std::string (" ] ");
    return (o);
}
