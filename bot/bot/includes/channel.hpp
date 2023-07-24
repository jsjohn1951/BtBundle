/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wismith <wismith@42ABUDHABI.AE>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 19:39:34 by wismith           #+#    #+#             */
/*   Updated: 2023/05/31 20:59:39 by wismith          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#	ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <vector>
# include <string>
# include <sys/socket.h>
# include <iostream>

namespace ft
{
	class channel
	{
		private :
			std::string					name;
			std::vector<std::string>	backlog;
			const int					&fd;

		public :
			channel (const int &f, const std::string &n);
			~channel ();

			void	Send();
			void	addPrivMsg(std::string str);
			void	addCmd(std::string str);
			void	addMsg(std::vector<std::string>	&);
			std::vector<std::string>	&retrieveBacklog();
	};
};

#endif