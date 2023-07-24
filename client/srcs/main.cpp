/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wismith <wismith@42ABUDHABI.AE>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/09 21:50:33 by wismith           #+#    #+#             */
/*   Updated: 2023/07/10 19:14:40 by wismith          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "../includes/client.hpp"

int	main(int argc, char **argv)
{
	int			port;

	if (argc < 4 || argc > 5)
	{
		std::cerr << "Incorrect num arguments" << std::endl;
		std::cout << "Format : " << std::endl;
		std::cout << "\t./client <IPv4> <port> [optional: username] <password>" << std::endl;
 		return (-1);
	}

	client	client;
	error	err;

	try{
		port = err.port(argv[2]);
	} catch (const std::exception &e)
	{
		std::cerr << e.what();
		return (-1);
	}

	client.init(port, argv[1]);
	if (argc == 5)
	{
		client.setUser(argv[3]);
		client.Connect(argv[4]);
	}
	else
		client.Connect(argv[3]);
	client.run();
}
