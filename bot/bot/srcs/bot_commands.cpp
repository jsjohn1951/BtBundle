/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot_commands.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wismith <wismith@42ABUDHABI.AE>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 17:31:18 by wismith           #+#    #+#             */
/*   Updated: 2023/07/24 21:01:28 by wismith          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/bot.hpp"

using namespace ft;

//! ------- COMMANDS --------- !//

void	bot::add(std::string &subUser, std::string &gen, const std::string &chan)
{
	if (this->channels.find(chan) == this->channels.end())
		return ;
	if (this->subj.find(subUser) != this->subj.end())
		return this->channels[chan]->addPrivMsg(subUser + " IS ALREADY A MEMBER!");
	this->channels[chan]->addPrivMsg("ADDING USER : " + subUser);
	this->subj[subUser] = new t_subj(this->bthBreaks, this->prarBreaks, (this->toLower(gen) == "f" ? FEMALE : MALE));
	this->log << subUser + " added to bot!";
}

void	bot::remove(std::string &subUser, const std::string &chan)
{
	if (this->channels.find(chan) == this->channels.end())
		return ;
	if (this->subj.find(subUser) == this->subj.end())
		return this->channels[chan]->addPrivMsg("NO INSTANCE OF \"" + subUser + "\" EXISTS!");
	this->channels[chan]->addPrivMsg("REMOVING USER : " + subUser);
	delete this->subj[subUser];
	this->subj.erase(this->subj.find(subUser));
	this->log << subUser + " removed from bot!";
}

void	bot::status(std::string &subUser, const std::string &chan)
{
	if (this->channels.find(chan) == this->channels.end())
		return ;
	if (this->subj.find(subUser) == this->subj.end())
		return this->channels[chan]->addPrivMsg("NO INSTANCE OF \"" + subUser + "\" EXISTS!");

	t_subj	*sub = this->subj[subUser];
	std::string	bth;
	std::string prar;
	std::string gender = (sub->gender == MALE ? "M" : "F");

	bth << sub->bth;
	prar << sub->prar;

	this->channels[chan]->addPrivMsg(subUser
								+ " status: [ "
								+ (sub->status == SEATED ?
								"SEATED" :
								sub->status == BTH_BREAK ?
								"WENT BATHROOM" :
								sub->status == PRAR_BREAK ?
								"WENT PRAYER BREAK" :
								sub->status == FINISHED ?
								"COMPLETED EXAM" :
								sub->status == EMG_BREAK ?
								"EMERGENCY" :
								"NO CLUE???")
								+ " ]");
	this->channels[chan]->addPrivMsg("Gender : [ "
								+ gender
								+ " ]");
	this->channels[chan]->addPrivMsg("BTH BREAKS LEFT : [ "
								+ bth
								+ " ]");
	this->channels[chan]->addPrivMsg("PRAR BREAKS LEFT : [ "
								+ prar
								+ " ]");
}

void	bot::bth(std::string &subUser, const std::string &chan)
{
	if (this->channels.find(chan) == this->channels.end())
		return ;
	if (this->subj.find(subUser) == this->subj.end())
		return this->channels[chan]->addPrivMsg("NO INSTANCE OF \"" + subUser + "\" EXISTS!");

	t_subj	*sub = this->subj[subUser];
	if (sub->status == FINISHED)
		return this->channels[chan]->addPrivMsg(subUser + " ALREADY FINISHED!");
	if (this->m_mode && (sub->gender == MALE ? track.mBth : track.fBth ) == 3)
		return this->channels[chan]->addPrivMsg("TOO MANY IN BATHROOM!");
	if (!sub->bth)
		return this->channels[chan]->addPrivMsg(subUser + " HAS NO MORE BATHROOM BREAKS LEFT!");
	if (sub->status == BTH_BREAK)
		return this->channels[chan]->addPrivMsg(subUser + " needs to come back first!");
	this->channels[chan]->addPrivMsg(subUser + " status set : [ WENT BATHROOM ]");
	sub->bth--;
	sub->status = BTH_BREAK;
	(sub->gender == MALE ? track.mBth : track.fBth )++;
	this->log << subUser + " : bathroom break";
}

void	bot::prar(std::string &subUser, const std::string &chan)
{
	if (this->channels.find(chan) == this->channels.end())
		return ;
	if (this->subj.find(subUser) == this->subj.end())
		return this->channels[chan]->addPrivMsg("NO INSTANCE OF \"" + subUser + "\" EXISTS!");

	t_subj	*sub = this->subj[subUser];
	if (sub->status == FINISHED)
	{
		this->log << subUser + " ALREADY FINISHED!";
		return this->channels[chan]->addPrivMsg(subUser + " ALREADY FINISHED!");
	}
	if (!sub->prar)
	{
		this->log << subUser + " : HAS NO MORE PRAYER BREAKS LEFT!";
		return this->channels[chan]->addPrivMsg(subUser + " HAS NO MORE PRAYER BREAKS LEFT!");
	}
	if (sub->status == PRAR_BREAK)
		return this->channels[chan]->addPrivMsg(subUser + " needs to come back first!");
	this->channels[chan]->addPrivMsg(subUser + " status set : [ WENT PRAYER BREAK ]");
	sub->prar--;
	sub->status = PRAR_BREAK;
	this->log << subUser + " : prayer break";
}

void	bot::emg(std::string &subUser, const std::string &chan)
{
	if (this->channels.find(chan) == this->channels.end())
		return ;
	if (this->subj.find(subUser) == this->subj.end())
		return this->channels[chan]->addPrivMsg("NO INSTANCE OF \"" + subUser + "\" EXISTS!");

	t_subj	*sub = this->subj[subUser];
	if (sub->status == FINISHED)
		return this->channels[chan]->addPrivMsg(subUser + " ALREADY FINISHED!");
	this->channels[chan]->addPrivMsg(subUser + " status set : [ EMERGENCY ]");
	sub->bth--;
	sub->status = EMG_BREAK;
	this->log << subUser + " : EMERGENCY";
}

void	bot::finish(std::string &subUser, const std::string &chan)
{
	if (this->channels.find(chan) == this->channels.end())
		return ;
	if (this->subj.find(subUser) == this->subj.end())
		return this->channels[chan]->addPrivMsg("NO INSTANCE OF \"" + subUser + "\" EXISTS!");
	
	t_subj	*sub = this->subj[subUser];
	if (sub->status == FINISHED)
	{
		this->log << subUser + " ALREADY FINISHED!";
		return this->channels[chan]->addPrivMsg(subUser + " ALREADY FINISHED!");
	}
	this->channels[chan]->addPrivMsg(subUser + " status set : [ FINISHED ]");
	sub->status = FINISHED;
	this->log << subUser + " : finished exam";
}

// void	bot::list( const std::string &chan)
// {
// 	if (this->channels.find(chan) == this->channels.end())
// 		return ;
// 	std::map<std::string, t_subj *>::iterator	it;

// 	for (it = this->subj.begin(); it != this->subj.end(); it++)
// 		this->channels[chan]->addPrivMsg(" [ " + it->first + " ]");
// }

void	bot::back(std::string &subUser, const std::string &chan)
{
	if (this->channels.find(chan) == this->channels.end())
		return ;
	if (this->subj.find(subUser) == this->subj.end())
		return this->channels[chan]->addPrivMsg("NO INSTANCE OF \"" + subUser + "\" EXISTS!");

	t_subj	*sub = this->subj[subUser];
	if (sub->status == FINISHED)
	{
		this->log << subUser + " ALREADY FINISHED!";
		return this->channels[chan]->addPrivMsg(subUser + " ALREADY FINISHED!");
	}
	if (sub->status == SEATED)
		return this->channels[chan]->addPrivMsg(subUser + " hasn't left yet!");
	this->channels[chan]->addPrivMsg(subUser + " status set : [ SEATED ]");
	sub->status = SEATED;
	if (this->m_mode)
		(sub->gender == MALE ? track.mBth : track.fBth )--;
	this->log << subUser + " status set : [ SEATED ]";
}

void	bot::numMsg( const std::string &chan, const std::string &MNum, const std::string &FNum)
{
	this->channels[chan]->addPrivMsg("[ M ] in room: " + std::string("[ ") + MNum + " ]");
	this->channels[chan]->addPrivMsg("[ F ] in room: " + std::string("[ ") + FNum + " ]");
}

void	bot::num( const std::string &chan )
{
	if (this->channels.find(chan) == this->channels.end())
		return ;

	std::string	tmp;
	std::string	tmp2;

	this->channels[chan]->addPrivMsg("Bathroom : ");

	tmp << this->track.mBth;
	this->numMsg(chan, (tmp << this->track.mBth), (tmp2 << this->track.fBth));
	tmp.clear();
	tmp2.clear();

	this->channels[chan]->addPrivMsg(" ");
	this->channels[chan]->addPrivMsg("Prayer : ");
	this->numMsg(chan, (tmp << this->track.mPrar), (tmp2 << this->track.fPrar));
}

void	bot::monitoring_mode( std::string &op, const std::string &chan )
{
	(void) chan;
	std::string	option;

	option = this->toLower(op);
	std::map<std::string, ft::channel *>::iterator	it = this->channels.begin();
	if (option == "on")
	{
		for (; it != this->channels.end(); it++)
			it->second->addPrivMsg("Monitoring mode switched on");
		this->m_mode = M_ON;
	}
	else if (option == "off")
	{
		for (; it != this->channels.end(); it++)
			it->second->addPrivMsg("Monitoring mode switched off");
		this->m_mode = M_OFF;
	}
}