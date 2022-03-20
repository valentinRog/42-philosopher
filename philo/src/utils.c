/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vrogiste <vrogiste@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/19 13:19:00 by vrogiste          #+#    #+#             */
/*   Updated: 2022/03/20 18:03:17 by vrogiste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	atoi_error(char *str, bool *error)
{
	long long int			sum;
	int						sign;
	bool					error_val;

	sum = 0;
	sign = 1;
	error_val = false;
	if (*str == '+' || *str == '-')
		if (*(str++) == '-')
			sign = -1;
	if (!*str)
		error_val = true;
	while (*str >= '0' && *str <= '9')
	{
		sum *= 10;
		sum += *str - '0';
		if ((sum * sign) > INT_MAX || (sum * sign) < INT_MIN)
			error_val = true;
		str++;
	}
	if (*str)
		error_val = true;
	if (error)
		*error = error_val;
	return (sum * sign);
}
