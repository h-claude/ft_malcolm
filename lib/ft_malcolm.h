/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malcolm.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hclaude <hclaude@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 16:06:51 by hclaude           #+#    #+#             */
/*   Updated: 2026/09/29 23:29:14 by hclaude          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MALCOLM_H
#define FT_MALCOLM_H

#include "turbo.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <net/if.h>
#include <ifaddrs.h>
#include <net/if_arp.h>

#define SOURCE_IP 0
#define SOURCE_MAC 0

#define TARGET_IP 1
#define TARGET_MAC 1

	typedef struct s_data
{
	struct in_addr		ip[2];
	unsigned char		mac[2][6];
	char				*interface;
	int					sockfd;
} t_data;

void	freetab(char **tab);

int		print_error(char *message);
int		print_invalid_ip(char *ip);
int		print_invalid_mac(char *mac);

int		hex_char_to_int(char c);
int		hex_pair_to_byte(char *s, unsigned char *out);

int		index_to_struct_index(int i);
int		check_and_put_IP(char **argv, int i, t_data *data);
int		check_and_put_MAC(char **argv, int i, t_data *data);
int		parse_input(char **argv, t_data *data);

void	print_data(t_data *data);

#endif