/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hclaude <hclaude@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 15:50:52 by hclaude           #+#    #+#             */
/*   Updated: 2026/09/21 19:44:29 by hclaude          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malcolm.h"

/*
sendto, recvfrom.
◦socket, setsockopt.
◦inet_pton, inet_ntop.
◦if_nametoindex, sleep.
◦getuid, close.
◦sigaction, signal.
◦inet_addr.
◦gethostbyname.
◦getaddrinfo, freeaddrinfo.
◦getifaddrs, freeifaddrs.
◦htons, ntohs.
◦strerror / gai_strerror.
◦printf and its family.

*/

int	launch_the_scam(t_data *data)
{
	// Get the good interface
	// Open the socket
	// listen ARP requests
	// check the request
	// create ARP reply
	// send ARP reply
	// stop the filouterie

	return (0);
}

// DONT FORGET TO MANAGE SIGACTION

int	main(int argc, char **argv)
{
	t_data	data;

	ft_bzero(&data, sizeof(t_data));

	if (argc != 5)
		return (print_error("Usage: ./ft_malcolm <source_ip> <source_mac> <target_ip> <target_mac>\n"));

	if (parse_input(++argv, &data))
		return (1);

	print_data(&data);

	if (launch_the_scam(&data))
		return (1);

	return (0); // Truc special a envoyer // j'ai oublie quoi
}
