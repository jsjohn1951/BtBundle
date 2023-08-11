#include "../../includes/parser.hpp"

s_ret::s_ret (): limits(false), breaks(false), names(false) {}

s_specs::s_specs (): limits(false), breaks(false), names(false), start(false) {}

parser::parser() {}
parser::~parser() {
    close (this->sock);
}

int        parser::getSock() const
{
    return (this->sock);
}

size_t     parser::getPort() const
{
    return (this->port);
}

void    parser::args (int argc, char **argv)
{
    if (argc != 4)
        throw (std::invalid_argument("args: Incorrect number of args!"));
    if ((this->sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        throw (std::invalid_argument("socket: Could not init socket!"));
    for (int i = 0; argv[2][i]; i++)
        if (!std::isdigit(argv[2][i]))
            throw (std::invalid_argument("port: Port should be all digits!"));
    this->port = std::atoi(argv[2]);

    struct sockaddr_in serv_addr;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(this->port);

    if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0)
        throw (std::invalid_argument("inet_pton: Given ip not correct!"));
    if (connect(this->sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        throw (std::invalid_argument("connect: Could not connect to given ip & port!"));
    if (fcntl(this->sock, F_SETFL, O_NONBLOCK) == -1)
        throw (std::invalid_argument("fcntl: Could not set to non-blocking!"));

    this->pass = std::string(argv[3]);
    this->pfds = NPOLL(this->sock);
}

void    parser::retrieveMsg ()
{
    std::ifstream infile(MSG);

    if (infile.good())
    {
        for (std::string line; getline(infile, line); )
            this->msg.push_back(line);
    }
    else
        throw (std::invalid_argument("infile: Could not open " + std::string(MSG)));
    infile.close();
}

void    parser::retrieveConfig ()
{
    std::ifstream infile(CONF);

    if (infile.good())
    {
        for (std::string line; getline(infile, line); )
            this->config.push_back(line);
    }
    else
        throw (std::invalid_argument("infile: Could not open " + std::string(CONF)));
    infile.close();
}

void    parser::eraseComments ()
{
    size_t  pos;
    size_t  end;
    bool    destroy = false;
    for (size_t i = 0; i < this->config.size(); i++)
    {
        if ((pos = this->config[i].find("#")) != this->config[i].npos)
            destroy = true;

        if ((end = this->config[i].find("`")) != this->config[i].npos)
        {
            if (pos > this->config[i].size())
                pos = 0;
            this->config[i].erase(pos, (end - pos + 1));
            destroy = false;
        }
        else if (destroy)
            this->config.erase(this->config.begin() + i--);
    }
     for (size_t i = 0; i < this->config.size(); i++)
        if (!this->config[i].size())
            this->config.erase(this->config.begin() + i--);
}

void    parser::printMsg()
{
    for (size_t i = 0; i < this->msg.size(); i++)
        std::cout << this->msg[i] << std::endl;
}

void    parser::printConf()
{
    std::cout << "Limits Prayer: " << this->limits.pray << std::endl;
    std::cout << "Limits bth: " << this->limits.bth << std::endl;
    std::cout << "breaks Prayer: " << this->breaks.pray << std::endl;
    std::cout << "breaks bth: " << this->breaks.bth << std::endl;
    std::cout << std::endl;
    this->printUsers();
}

void							parser::pRecv(const std::string &cmd)
{
	std::string	str = cmd;
	size_t	pos = str.npos;

	while ((pos = str.find('\r', pos + 1)) != str.npos)
		str.erase(pos, 1);

	std::stringstream			ss(str);
	std::string					line;

	while (!ss.eof())
	{
		getline(ss, line, '\n');
		this->cmds.push_back(line);
	}
}

void	parser::Read()
{
	char	buff[513];
	std::string	res;
	ssize_t bits = recv(this->sock, buff, 512, 0);
	if (!bits || bits > 512)
		return ;

	buff[bits] = '\0';
	std::string	str(buff);
	if (bits > 0)
	{
		if (str.find('\n') == str.npos && str.find('\r') == str.npos)
			this->buffer += str;
		else
		{
			res = this->buffer + str;
			this->buffer.clear();
			this->pRecv(res);
		}
	}
}

std::vector<std::string>		&parser::getCmdSec(size_t i)
{
    this->cmd_sec.clear();
	std::stringstream			ss(this->cmds[i]);
	std::string					word;

	while (!ss.eof())
	{
		getline(ss, word, ' ');
		this->cmd_sec.push_back(word);
	}
    return (this->cmd_sec);
}
