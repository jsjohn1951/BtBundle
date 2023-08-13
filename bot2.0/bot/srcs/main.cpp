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

int main(int argc, char **argv)
{
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