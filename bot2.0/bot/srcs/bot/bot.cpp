#include "../../includes/bot.hpp"

bool runner = true;

bot::bot () : backlog(), Log("bot.log") {
    this->mBasic["help"] = &bot::help;
    this->mBasic["num"] = &bot::num;
    this->mBasic["list"] = &bot::list;

    this->stdFuncs["status"] = &bot::status;
    this->stdFuncs["remove"] = &bot::remove;
    this->stdFuncs["bth"] = &bot::bth;
    this->stdFuncs["prar"] = &bot::prar;
    this->stdFuncs["back"] = &bot::back;
    this->stdFuncs["emg"] = &bot::emg;
}

bot::~bot () {}

void    bot::init () {
    this->backlog.push_back("CAP LS\r\n");
	this->backlog.push_back("PASS " + this->pass + "\r\n");
	this->backlog.push_back("NICK bot\r\n");
	this->backlog.push_back("USER bot bot localhost :ircbot\r\n");
	this->backlog.push_back("CAP END\r\n");
}

void    bot::run () {

    this->init_signals ();
    while (runner)
    {
        poll (&this->pfds, 1, -1);
        if (this->pfds.revents & POLLOUT && this->backlog.size())
        {
            for (size_t i = 0; i < this->backlog.size(); i++)
            {
                send (this->sock, this->backlog[i].c_str(), this->backlog[i].size(), 0);
                usleep (15000);
            }
            this->backlog.clear();
        }


        if (this->pfds.revents & POLLIN)
        {
            this->Read();
            for (size_t i = 0; i < this->cmds.size(); i++)
                this->cmdsel(this->getCmdSec(i));
        }
        this->cmds.clear();
    }
    if (this->pfds.revents & POLLOUT)
		send(this->sock, "QUIT :bot is leaving!\r\n",
            sizeof("QUIT :bot is leaving!\r\n"), 0);
    close(this->sock);
}

void    bot::cmdsel(std::vector<std::string> &vec)
{
    for (size_t i = 0; i < vec.size(); i++)
        std::cout << i << ") " << vec[i] << std::endl;
    std::cout << std::endl;

    std::cout << "Size: " << vec.size() << std::endl;
    if (vec.size() < 4)
        return ;
    if (vec[1] == "INVITE")
        this->invite(vec[3]);

    if (vec[1] != "PRIVMSG" || vec[3] != ":bot")
        return ;

    if (vec[4] == "add" && vec.size() >= 7)
        return (this->add(vec[5], vec[6], vec[2]));

    try{
		if (vec.size() == 5)
		{
    		if (this->mBasic.find(vec[4]) != this->mBasic.end())
        		return ((this->*mBasic[vec[4]]) (vec[2]));
			else
				throw (std::invalid_argument("Unknown Command '" + vec[4] + "'"));
		}
		else
		{
        	if (this->stdFuncs.find(vec[4]) != this->stdFuncs.end())
            	(this->*stdFuncs[vec[4]]) (vec[5], vec[2]);
			else
				throw (std::invalid_argument("Unknown Command '" + vec[4] + "'"));
		}
    }
    catch (const std::exception &e)
    {
		this->privMsg (vec[2], ":runtime:\x1B[31m " + std::string(e.what()) + "\x1B[0m");
		this->privMsg (vec[2], ":runtime: Run 'bot help' for available commands");
	}
}

void    sighandlr(int signum)
{
    (void) signum;
    runner = false;
    std::cout << "\nStopping bot!" << std::endl;
}

void    bot::init_signals()
{
    signal(SIGINT, sighandlr);
	signal(SIGQUIT, sighandlr);
	signal(SIGHUP, sighandlr);
	signal(SIGTERM, sighandlr);
	signal(SIGPIPE, sighandlr);
}

void    bot::logSub(t_subj &sub)
{
    std::string tmp;
    tmp << sub;
    this->Log << tmp
		+ (sub.status != SEATED && sub.status != PRAYER ? "" : "\t")
		+ "Time Elapsed: [ "
		+ (sub.status != SEATED ? "00:00:00" : this->usrCurElapTime(sub))
		+ " ] ";
}
