/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vrogiste <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 09:57:52 by vrogiste          #+#    #+#             */
/*   Updated: 2022/03/24 11:10:56 by vrogiste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	process(t_param *param, int index)
{
	sem_wait(param->sem_ready);
	sem_post(param->sem_ready);
	uint64_t	t_zero = get_time();
	while (true)
	{
		sem_wait(param->sem_forks);
		sem_wait(param->sem_forks);
		printf("%" PRId64 " %d is eating\n",get_time() - t_zero, index + 1);
		micro_sleep(param->time_to_eat);
		sem_post(param->sem_forks);
		sem_post(param->sem_forks);
		printf("%" PRId64 " %d is sleeping\n",get_time() - t_zero, index + 1);
		micro_sleep(param->time_to_sleep);
		printf("%" PRId64 " %d is thinking\n",get_time() - t_zero, index + 1);
	}
	exit(EXIT_SUCCESS);
}
