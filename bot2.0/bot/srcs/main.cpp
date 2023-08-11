#include "../includes/bot.hpp"
#include "../includes/log.hpp"

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

//    ./bot 127.0.0.1 6667 bob

// std::string	time_convert(time_t	&time)
// {
// 	std::string timeElapsed;
// 	timeElapsed = ((time / (60 * 60)) < 10 ? "0" : "")
// 		+ std::to_string ((time / (60 * 60)))
// 		+ ":"
// 		+ (((time / 60) % 60) < 10 ? "0" : "")
// 		+ std::to_string (((time / 60) % 60))
// 		+ ":"
// 		+ ((time % 60) < 10 ? "0" : "")
// 		+ std::to_string (time % 60);
// 		return (timeElapsed);
// }

int main(int argc, char **argv)
{
	// std::chrono::system_clock::time_point tm = std::chrono::system_clock::now();
	// std::time_t		current = std::chrono::system_clock::to_time_t(tm);

	// sleep (4);

	// std::chrono::system_clock::time_point tm2 = std::chrono::system_clock::now();
	// std::time_t		after = std::chrono::system_clock::to_time_t(tm2);

	// after -= current;
	// std::cout << time_convert(after) << std::endl;
	// exit (0);

    try
    {
        bot Bot;
        Bot.args(argc, argv);
        Bot.retrieveMsg();
        Bot.retrieveConfig();
        Bot.eraseComments();
        Bot.limitDis();
        Bot.init();
        // Bot.printMsg();
        Bot.run();
        // Bot.printConf();
    }
    catch (const std::invalid_argument &e)
    {
        std::cout << KRED;
        std::cerr << "Bot: " << e.what() << std::endl;
        std::cout << KNRM;
        exit (1);
    }
    return (0);
}