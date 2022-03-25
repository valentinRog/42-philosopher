/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vrogiste <vrogiste@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 09:57:52 by vrogiste          #+#    #+#             */
/*   Updated: 2022/03/25 13:26:22 by vrogiste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*death_loop(void *args)
{
	t_param	*param = (t_param *)args;

	while (true)
	{
		sem_wait(param->sem_last_eat);
		
		if (get_time() - param->last_eat > (uint64_t)param->time_to_die)
		{
			sem_post(param->sem_last_eat);
			sem_wait(param->sem_print);
			printf("%" PRId64"%d died\n", get_time(), param->index);
			exit(EXIT_SUCCESS);
		}
		sem_post(param->sem_last_eat);
	}
	return (NULL);
}

void	process(t_philo *philo)
{
	sem_wait(philo->param->sem_ready);
	sem_post(philo->param->sem_ready);
	printf("%d\n", philo->index + 1);
}
