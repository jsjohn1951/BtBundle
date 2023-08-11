#include "../../includes/bot.hpp"

void    bot::addCmd(const std::string &cmd)
{
    // std::cout << "cmd: " << cmd << std::endl;
    this->backlog.push_back(cmd + "\r\n");
}

void    bot::privMsg(const std::string &chan, const std::string &msg)
{
    this->addCmd("PRIVMSG " + chan + " " + msg);
}

void    bot::invite(const std::string &chan)
{
    this->addCmd("JOIN " + std::string(chan.c_str() + 1));
}

std::map<std::string, t_subj>::iterator bot::findUser(const std::string &usr)
{
    std::map<std::string, t_subj>::iterator it;
    if ((it = this->sub.find(usr)) == this->sub.end())
        throw std::invalid_argument ("User not found!");
    return (it);
}

void	bot::preSet(int preset, int flag)
{
	if (preset != SEATED && flag != SEATED)
		throw std::invalid_argument ("Wait for person to return");
	else if (preset == flag)
		throw std::invalid_argument ("Person already seated");
}

std::string	bot::usrElapTime(t_subj &usr)
{
	std::chrono::system_clock::time_point tm = std::chrono::system_clock::now();
	std::time_t	nTime = std::chrono::system_clock::to_time_t(tm);
	std::time_t	sav = nTime - usr.elapsed;
	usr.elapsed = nTime;
	return (this->Log.time_convert(sav));
}

void    bot::add (const std::string &sub, const std::string &gen, const std::string &chan)
{
    this->privMsg(chan, ":Adding '" + sub + "' to bot");
    this->sub[sub] = (t_subj) {sub,
                                ( gen == "F" ? FEMALE : MALE),
                                    SEATED,
                                    {this->breaks.pr,
                                    this->breaks.bt,
                                    this->breaks.pray,
                                    this->breaks.bth}, 0};
    this->Log << sub + " was added to the bot";
}

void    bot::remove(const std::string &usr, const std::string &chan)
{
    std::map<std::string, t_subj>::iterator it = this->findUser(usr);
    this->privMsg(chan, ":removing " + usr);
    this->sub.erase(it);
    this->Log << usr + " was removed from bot";
}

// list / num / help

void    bot::list(const std::string &chan)
{
    std::map<std::string, t_subj>::iterator it;
    for (it = this->sub.begin(); it != this->sub.end(); it++)
    {
        std::string tmp;
        tmp << it->second;
        tmp += "bth:" + std::string("[ ") + std::to_string(it->second.breaks.bth) + " ] ";
        tmp += "prar:" + std::string("[ ") + std::to_string(it->second.breaks.pray) + " ] ";
        this->privMsg(chan, ":" + tmp);
    }
}

void    bot::num (const std::string &chan)
{
    std::map<std::string, t_subj>::iterator it;
    this->privMsg(chan, ":");
    this->privMsg(chan, ":in Bathroom :");
    for (it = this->sub.begin(); it != this->sub.end(); it++)
    {
        if (it->second.status != BATHROOM)
            continue;
        std::string tmp;
        this->privMsg(chan, ":" + (tmp << it->second));
    }

    this->privMsg(chan, ":");
    this->privMsg(chan, ":in prayer :");
    for (it = this->sub.begin(); it != this->sub.end(); it++)
    {
        if (it->second.status != PRAYER)
            continue;
        std::string tmp;
        this->privMsg(chan, ":" + (tmp << it->second));
    }
}

void    bot::help (const std::string &chan)
{
    for (size_t i = 0; i < this->msg.size(); i++)
        this->privMsg(chan, ":" + this->msg[i]);
}

void    bot::status (const std::string &usr, const std::string &chan)
{
    std::map<std::string, t_subj>::iterator it = this->findUser(usr);
    std::string tmp;
    tmp << it->second;
    tmp += "bth:" + std::string("[ ") + std::to_string(it->second.breaks.bth) + " ] ";
    tmp += "prar:" + std::string("[ ") + std::to_string(it->second.breaks.pray) + " ] ";
    this->privMsg(chan, ":" + tmp);
}

void    bot::bth (const std::string &usr, const std::string &chan)
{
    std::map<std::string, t_subj>::iterator it = this->findUser(usr);
	this->preSet(it->second.status, BATHROOM);
    if (!it->second.breaks.bth)
        return (this->privMsg(chan, ":No more breaks available!"));
    it->second.breaks.bth--;
    it->second.status = BATHROOM;
	this->usrElapTime(it->second);
    this->privMsg(chan, ":" + usr + " status set to bth");
    this->logSub(it->second);
}

void    bot::prar (const std::string &usr, const std::string &chan)
{
    std::map<std::string, t_subj>::iterator it = this->findUser(usr);
	this->preSet(it->second.status, PRAYER);
    if (!it->second.breaks.pray)
        return (this->privMsg(chan, ":No more breaks available!"));
    it->second.breaks.pray--;
    it->second.status = PRAYER;
	this->usrElapTime(it->second);
    this->privMsg(chan, ":" + usr + " status set to pray");
    this->logSub(it->second);
}

void    bot::emg (const std::string &usr, const std::string &chan)
{
    std::map<std::string, t_subj>::iterator it = this->findUser(usr);
	this->preSet(it->second.status, EMERGENCY);
    it->second.status = EMERGENCY;
    this->privMsg(chan, ":" + usr + " status set to EMERGENCY");
    this->logSub(it->second);
}

void    bot::back (const std::string &usr, const std::string &chan)
{
    std::map<std::string, t_subj>::iterator it = this->findUser(usr);
	this->preSet(it->second.status, SEATED);
    it->second.status = SEATED;

    this->privMsg(chan, ":" + usr + " status set to SEATED");
	this->privMsg(chan, ":" + usr + " time elapsed: " + this->usrElapTime(it->second));
    this->logSub(it->second);
}
