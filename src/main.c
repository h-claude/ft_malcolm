/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hclaude <hclaude@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 15:50:52 by hclaude           #+#    #+#             */
/*   Updated: 2026/09/26 21:59:42 by hclaude          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malcolm.h"

/*
sendto        : sends data through a socket, used here to send a crafted ARP reply.
recvfrom      : receives data from a socket, used here to read incoming ARP packets.
socket        : creates an endpoint for communication (opens a raw socket).		 **
setsockopt    : sets options on a socket (e.g. bind it to a specific interface). **
inet_pton     : converts a human-readable IP address (text) into its binary form.
inet_ntop     : converts a binary IP address back into human-readable text.
if_nametoindex: gets the interface index number from its name (e.g. "eth0").
sleep         : pauses program execution for a given number of seconds.
getuid        : returns the real user ID of the calling process (used to check for root).
close         : closes a file descriptor (e.g. the socket).
sigaction     : sets up a handler for a given signal, more configurable than signal().
signal        : sets a simple handler for a given signal (e.g. SIGINT).
inet_addr     : converts an IPv4 address string to its binary (network byte order) form.
gethostbyname : resolves a hostname to an IP address (legacy DNS lookup).
getaddrinfo   : resolves a hostname/service to a list of address structures (modern DNS lookup).
freeaddrinfo  : frees the memory allocated by getaddrinfo.
getifaddrs    : retrieves a list of the machine's network interfaces and their addresses.
freeifaddrs   : frees the memory allocated by getifaddrs.
htons         : converts a 16-bit value from host byte order to network byte order.
ntohs         : converts a 16-bit value from network byte order to host byte order.
strerror      : returns a human-readable string describing an errno error code.
gai_strerror  : returns a human-readable string describing a getaddrinfo() error code.
printf family : formatted output functions (printf, fprintf, snprintf, ...).
*/

static int	is_candidate_link(struct ifaddrs *ifa)
{
	if (!ifa->ifa_addr || ifa->ifa_addr->sa_family != AF_LINK)
		return (0);
	if (!(ifa->ifa_flags & IFF_UP) || !(ifa->ifa_flags & IFF_BROADCAST))
		return (0);
	if (ifa->ifa_flags & IFF_POINTOPOINT || ifa->ifa_flags & IFF_LOOPBACK)
		return (0);
	return (((struct sockaddr_dl *)ifa->ifa_addr)->sdl_type == IFT_ETHER);
}

static int	has_valid_ipv4(struct ifaddrs *list, const char *name)
{
	struct sockaddr_in	*sin;

	while (list)
	{
		if (ft_strncmp(list->ifa_name, name, ft_strlen(name)) == 0
			&& list->ifa_addr && list->ifa_addr->sa_family == AF_INET)
		{
			sin = (struct sockaddr_in *)list->ifa_addr;
			if (sin->sin_addr.s_addr != 0)
				return (1);
		}
		list = list->ifa_next;
	}
	return (0);
}

static char	*find_interface(struct ifaddrs *list)
{
	struct ifaddrs	*it;

	it = list;
	while (it)
	{
		if (is_candidate_link(it) && has_valid_ipv4(list, it->ifa_name))
			return (it->ifa_name);
		it = it->ifa_next;
	}
	return (NULL);
}

int	launch_the_scam(t_data *data)
{
	struct ifaddrs	*list;
	char			*interface;
	unsigned int	index;

	if (getifaddrs(&list) == -1)
		return (print_error(strerror(errno)));
	interface = find_interface(list);
	if (!interface)
	{
		freeifaddrs(list);
		return (print_error("No suitable network interface found\n"));
	}
	printf("Selected interface: %s\n", interface);
	index = if_nametoindex(interface);
	if (index == 0)
		return (print_error(strerror(errno)));
	(void)data;
	freeifaddrs(list);

	int sock = socket(PF_LOCAL, SOCK_RAW, SOCK_RAW);
	setsockopt(sock, )

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
