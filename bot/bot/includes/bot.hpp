/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wismith <wismith@42ABUDHABI.AE>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 15:45:00 by wismith           #+#    #+#             */
/*   Updated: 2023/07/24 20:54:02 by wismith          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#	ifndef BOT_HPP
# define BOT_HPP

/** @brief bot message to be displayed when 'bot help'
 * is requested by client
*/
# define MSGFILE std::string("msg.bot")

/** @brief bot configuration */
# define CONFIG	std::string("bot.conf")

# define NPOLL(nfd) (struct pollfd){ .fd = nfd, .events = POLLIN | POLLOUT, .revents = 0 }

# include <poll.h>
# include <stdio.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <signal.h>
# include <vector>
# include <map>
# include <string>
# include <fstream>

# include "../ftServerUtils/includes/ServerUtility.hpp"
# include "parser.hpp"
# include "channel.hpp"

# define SEATED 0
# define BTH_BREAK 1
# define PRAR_BREAK 2
# define FINISHED 3
# define EMG_BREAK 4

# define MALE 0
# define FEMALE 1

#define BREAKS 1
#define NAMES 2
#define LIMITS 3

#define M_ON true
#define M_OFF false

# define MAPPY std::map<std::string, ft::channel *>

typedef struct s_track_num
{
	int	mBth; //! Current number in bth
	int	mPrar; //! Current number in prar

	int	fBth; //! Current number in bth female
	int	fPrar; //! Current number in prar female

	s_track_num ();
	~s_track_num ();
}	t_track;

namespace ft
{
	typedef struct s_subj
	{
		int	bth;
		int	prar;
		int	status;
		int	gender;

		s_subj (int bth_q, int prar_q, int gen);
		~s_subj ();
	}	t_subj;

	class bot
	{
		private :
			ft::log	log;
			int										fd;
			struct pollfd							pfd;
			std::string								buffer;
			ft::parser								pars;
			std::vector<std::string>				msgs;
			std::vector<std::string>				backlog;
			std::vector<std::string>				chan;
			std::map<std::string, ft::channel *>	channels;
			t_track 								track;
			std::map<std::string, void (bot::*)(std::string &, const std::string &)>	funcs;
			bool									m_mode;


			int										bthBreaks;
			int										prarBreaks;

			std::map<std::string, t_subj *>			subj;

			void									add(std::string &subUser, std::string &gen, const std::string &chan);

			void									remove(std::string &subUser, const std::string &chan);
			void									status(std::string &subUser, const std::string &chan);
			void									bth(std::string &subUser, const std::string &chan);
			void									prar(std::string &subUser, const std::string &chan);
			void									emg(std::string &subUser, const std::string &chan);
			void									finish(std::string &subUser, const std::string &chan);
			void									back(std::string &subUser, const std::string &chan);
			void									monitoring_mode( std::string &chan, const std::string &op );

			// void									list( const std::string &chan);
			void									num( const std::string &chan );

			void									numMsg( const std::string &chan, const std::string &MNum, const std::string &FNum);

			void									catch_signals();
			std::string								Read(int fd);
			void									selCmd(std::vector<std::string>);
			std::string								toLower(const std::string);

			/** @brief parse config */
			void									bot_Break(const std::string &line);
			void									bot_NAMES(const std::string &line);

		public	:
													bot ();
													~bot ();

			void									init (int, const std::string);
			void									Connect(std::string);
			void									run ();

			void									retrieveConfig();
			void									retrieveMsgs();
	};
};

#endif