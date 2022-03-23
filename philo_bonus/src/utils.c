/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vrogiste <vrogiste@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/19 13:19:00 by vrogiste          #+#    #+#             */
/*   Updated: 2022/03/23 07:02:13 by vrogiste         ###   ########.fr       */
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

uint64_t	get_time(void)
{
	static struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * (uint64_t)1000) + (tv.tv_usec / 1000));
}

void	micro_sleep(uint64_t time)
{
	uint64_t	i;

	i = get_time();
	while (true)
	{
		if (get_time() - i >= time)
			break ;
		usleep(50);
	}
}
