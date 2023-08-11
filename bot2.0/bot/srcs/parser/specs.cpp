#include "../../includes/parser.hpp"

 std::pair<std::string, std::string> parser::ft_split(std::string &str, const std::string &delims)
{
    std::pair<std::string, std::string> pr;

    pr.first = std::string(std::strtok(const_cast<char *>(str.c_str()), delims.c_str()));
    pr.second = std::string(std::strtok(NULL, delims.c_str()));
    return (pr);
}

/** Parse config */
void    parser::names(t_ret &ret, std::string &line)
{
    (void) ret;
    std::pair<std::string, std::string> pr;
    pr = this->ft_split(line, "\n\t ");
    this->sub[pr.first] = (t_subj) {pr.first,
                                    (pr.second == "F" ? FEMALE : MALE),
                                        SEATED,
                                        {this->breaks.pr,
                                        this->breaks.bt,
                                        this->breaks.pray,
                                        this->breaks.bth}, 0};
}

void    Specs(t_specs &specs, t_ret &ret, int type)
{
	t_specs	spec;
	spec.start = specs.start;
    switch (type)
    {
        case LIMITS :
			spec.limits = true;
            ret.limits = true;
            break ;
        case BREAKS :
			spec.breaks = true;
            ret.breaks = true;
            break ;
        case NAMES :
			spec.names = true;
            ret.names = true;
            break ;
    }
	specs = spec;
}

void parser::Overlord( const std::string &line, t_param &param)
{
    size_t  pos_w = 0, pos_eq = 0;
    if ((pos_w = line.find("PRAY")) != line.npos)
    {
        if ((pos_eq = line.find("=")) != line.npos && pos_w < pos_eq)
        {
            param = (t_param) {true,
                        param.bt,
                        std::atoi(line.c_str() + pos_eq + 1),
                        param.bth};
        }
        else
            throw (std::invalid_argument("Config: Syntax error '='"));
    }
    if ((pos_w = line.find("BTH")) != line.npos)
    {
        if ((pos_eq = line.find("=")) != line.npos && pos_w < pos_eq)
            param = (t_param) {param.pr,
                        true,
                        param.pray,
                        std::atoi(line.c_str() + pos_eq + 1)};
        else
            throw (std::invalid_argument("Config: Syntax error '='"));
    }
}

void parser::limitDis()
{
    size_t  i = 0, pos = 0;
    t_ret   ret;
    t_specs specs;

    for (; i < this->config.size(); i++)
    {
        if ((pos = this->config[i].find("LIMITS")) != this->config[i].npos)
            Specs(specs, ret, LIMITS);
        else if ((pos = this->config[i].find("BREAKS")) != this->config[i].npos)
            Specs(specs, ret, BREAKS);
        else if ((pos = this->config[i].find("NAMES")) != this->config[i].npos)
        {
            Specs(specs, ret, NAMES);
            continue ;
        }

        (specs.limits ?
            Overlord( this->config[i], this->limits) :
        specs.breaks ?
            Overlord( this->config[i], this->breaks) :
            (void) this->config[i]);

        if (specs.names && ret.breaks)
            names (ret, this->config[i]);
    }

    if (!ret.limits)
        throw (std::invalid_argument("Config: No Limits specified"));
    else if (!ret.breaks)
        throw (std::invalid_argument("Config: No Breaks specified"));
}

void    parser::printUsers ()
{
    std::map<std::string, t_subj>::iterator it = this->sub.begin();

    for (; it != this->sub.end(); it++)
    {
        std::cout << "Name: " << it->first << std::endl;
        std::cout << "bth: " << it->second.breaks.bth << " " << it->second.breaks.bt << std::endl;
        std::cout << "pray: " << it->second.breaks.pray << " " << it->second.breaks.pr << std::endl;
        std::cout << "Gender: " << (it->second.gen == MALE ? "Male" : "Female") << std::endl;
        std::cout << std::endl;
    }
}