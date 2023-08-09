#   ifndef BOT_HPP
# define BOT_HPP

#include "parser.hpp"
#include "config.hpp"
#include "log.hpp"

class bot : public parser
{
    private :
        std::vector<std::string>                                                        backlog;
        log                                                                             Log;
        std::map<std::string, void (bot::*)(const std::string &)>                       mBasic;
        std::map<std::string, void (bot::*)(const std::string &, const std::string&)>   stdFuncs;

    public :
                                                                                        bot();
                                                                                        ~bot();

        void                                                                            init ();
        void                                                                            run ();
        void                                                                            init_signals();

        void                                                                            logSub(t_subj &sub);
        void                                                                            cmdsel(std::vector<std::string> &);
        void                                                                            addCmd(const std::string &);
        void                                                                            privMsg(const std::string &, const std::string &);

    // bot commands
        void                                                                            help(const std::string &);
        void                                                                            num(const std::string &);
        void                                                                            list(const std::string &);

        void                                                                            invite(const std::string &);
        void                                                                            add (const std::string &, const std::string &, const std::string &);

        void                                                                            remove (const std::string &, const std::string &);
        void                                                                            status (const std::string &, const std::string &);
        void                                                                            bth (const std::string &, const std::string &);
        void                                                                            prar (const std::string &, const std::string &);
        void                                                                            back (const std::string &, const std::string &);
        void                                                                            emg (const std::string &, const std::string &);

    // bot errors throws exceptions
        std::map<std::string, t_subj>::iterator                                         findUser(const std::string &);

};

#endif