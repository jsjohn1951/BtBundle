/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_config.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wismith <wismith@42ABUDHABI.AE>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 17:35:10 by wismith           #+#    #+#             */
/*   Updated: 2023/07/22 13:01:49 by wismith          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/bot.hpp"

using namespace ft;

void	bot::bot_Break(const std::string &str)
{
	std::stringstream			ss(str);
	std::string					line;
	std::vector<std::string>	vec;

	while (!ss.eof())
	{
		getline(ss, line, ' ');
		vec.push_back(line);
	}

	if (vec.size() == 2)
	{
		if (vec[0] == "BTH")
			this->bthBreaks = std::atoi(vec[1].c_str());
		else if (vec[0] == "PRAR")
			this->prarBreaks = std::atoi(vec[1].c_str());
	}
}

void	bot::bot_NAMES(const std::string &str)
{
	std::stringstream			ss(str);
	std::string					line;
	std::vector<std::string>	vec;

	while (!ss.eof())
	{
		getline(ss, line, ' ');
		vec.push_back(line);
	}

	if (vec.size() == 1)
		this->subj[vec[0]]
			= new t_subj(this->bthBreaks, this->prarBreaks, MALE);
	else if (vec.size() > 1)
		this->subj[vec[0]]
			= new t_subj(this->bthBreaks, this->prarBreaks, (this->toLower(vec[1]) == "f" ? FEMALE : MALE));
}

void	bot::retrieveConfig()
{
	std::ifstream	infile(CONFIG.c_str());
	int				flags = 0;

	if (!infile.good())
	{
		std::cout << "No file exists!\n";
		return ;
	}

	for (std::string line; getline(infile, line);)
	{
		if (line.size())
		{
			flags = ( line == "BOT BREAKS" ? BREAKS :
					line == "BOT NAMES" ? NAMES : 
					line == "BOT LIMITS" ? LIMITS :
					line == "END" ? 0 :
					flags );

			switch (flags)
			{
				case BREAKS :
					this->bot_Break(line);
				break;
				case NAMES :
					if ( line != "BOT NAMES" && line.find("#") == line.npos )
						this->bot_NAMES(line);
				break;
				case LIMITS :
				break;
			}
		}
	}
	infile.close();
}
