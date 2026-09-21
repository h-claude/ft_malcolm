/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_data.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hclaude <hclaude@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 19:40:00 by hclaude           #+#    #+#             */
/*   Updated: 2026/09/21 19:40:00 by hclaude          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malcolm.h"

void	print_data(t_data *data)
{
	printf("Source IP: %s\n", inet_ntoa(data->ip[SOURCE_IP]));
	printf("Source MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
		data->mac[SOURCE_MAC][0], data->mac[SOURCE_MAC][1],
		data->mac[SOURCE_MAC][2], data->mac[SOURCE_MAC][3],
		data->mac[SOURCE_MAC][4], data->mac[SOURCE_MAC][5]);
	printf("Target IP: %s\n", inet_ntoa(data->ip[TARGET_IP]));
	printf("Target MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
		data->mac[TARGET_MAC][0], data->mac[TARGET_MAC][1],
		data->mac[TARGET_MAC][2], data->mac[TARGET_MAC][3],
		data->mac[TARGET_MAC][4], data->mac[TARGET_MAC][5]);
}
