/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vrogiste <vrogiste@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/19 13:19:00 by vrogiste          #+#    #+#             */
/*   Updated: 2022/03/21 19:15:37 by vrogiste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	atoi_error(char *str, bool *error)
{
	uint32_t	sum;
	bool		error_val;

	sum = 0;
	error_val = false;
	if (!*str)
		error_val = true;
	while (*str >= '0' && *str <= '9')
	{
		sum *= 10;
		sum += *str - '0';
		if (sum > INT_MAX)
			error_val = true;
		str++;
	}
	if (*str)
		error_val = true;
	if (error)
		*error = error_val;
	return (sum);
}
