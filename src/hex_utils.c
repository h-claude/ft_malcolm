/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hex_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hclaude <hclaude@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 19:40:00 by hclaude           #+#    #+#             */
/*   Updated: 2026/09/21 19:40:00 by hclaude          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malcolm.h"

int	hex_char_to_int(char c)
{
	if (c >= '0' && c <= '9')
		return (c - '0');
	if (c >= 'a' && c <= 'f')
		return (c - 'a' + 10);
	if (c >= 'A' && c <= 'F')
		return (c - 'A' + 10);
	return (-1);
}

int	hex_pair_to_byte(char *s, unsigned char *out)
{
	int	high;
	int	low;

	high = hex_char_to_int(s[0]);
	low = hex_char_to_int(s[1]);
	if (high == -1 || low == -1)
		return (1);
	*out = (unsigned char)(high * 16 + low);
	return (0);
}
