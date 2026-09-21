/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hclaude <hclaude@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 19:40:00 by hclaude           #+#    #+#             */
/*   Updated: 2026/09/21 19:40:00 by hclaude          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malcolm.h"

int	index_to_struct_index(int i)
{
	if (i == 0)
		return (SOURCE_IP);
	if (i == 1)
		return (SOURCE_MAC);
	if (i == 2)
		return (TARGET_IP);
	if (i == 3)
		return (TARGET_MAC);
	return (-1);
}

int	check_and_put_IP(char **argv, int i, t_data *data)
{
	int	c;

	c = index_to_struct_index(i);
	if (c == -1)
		return (1);
	if (inet_pton(AF_INET, argv[i], &data->ip[c]) != 1)
		return (print_invalid_ip(argv[i]));
	return (0);
}

int	check_and_put_MAC(char **argv, int i, t_data *data)
{
	char	**mac_parts;
	char	*original;
	int		c;

	c = index_to_struct_index(i);
	if (c == -1)
		return (1);
	original = argv[i];
	mac_parts = ft_split(argv[i], ':');
	if (!mac_parts)
		return (print_invalid_mac(original));
	i = 0;
	while (mac_parts[i])
	{
		if (strlen(mac_parts[i]) != 2
			|| hex_pair_to_byte(mac_parts[i], &data->mac[c][i]))
		{
			freetab(mac_parts);
			return (print_invalid_mac(original));
		}
		i++;
	}
	freetab(mac_parts);
	if (i != 6)
		return (print_invalid_mac(original));
	return (0);
}

int	parse_input(char **argv, t_data *data)
{
	int	i;

	i = 0;
	while (i <= 3)
	{
		if (i % 2 == 0)
		{
			if (check_and_put_IP(argv, i, data))
				return (1);
		}
		else
		{
			if (check_and_put_MAC(argv, i, data))
				return (1);
		}
		i++;
	}
	return (0);
}
