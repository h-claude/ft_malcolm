/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hclaude <hclaude@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 19:40:00 by hclaude           #+#    #+#             */
/*   Updated: 2026/09/21 19:40:00 by hclaude          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malcolm.h"

int	print_error(char *message)
{
	ft_putstr_fd(message, STDERR_FILENO);
	return (1);
}

int	print_invalid_ip(char *ip)
{
	fprintf(stderr, "ft_malcolm: unknown host or invalid IP address: (%s).\n",
		ip);
	return (1);
}

int	print_invalid_mac(char *mac)
{
	fprintf(stderr, "ft_malcolm: invalid mac address: (%s).\n", mac);
	return (1);
}
