/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vrogiste <vrogiste@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/19 13:19:00 by vrogiste          #+#    #+#             */
/*   Updated: 2022/03/22 10:14:45 by vrogiste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	atoi_error(char *str, int *nb)
{
	uint64_t	sum;

	sum = 0;
	if (!*str)
		return (true);
	while (*str >= '0' && *str <= '9')
	{
		sum *= 10;
		sum += *str - '0';
		if (sum > INT_MAX)
			return (true);
		str++;
	}
	if (*str)
		return (true);
	if (nb)
		*nb = sum;
	return (false);
}
