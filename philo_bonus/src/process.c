/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vrogiste <vrogiste@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 09:57:52 by vrogiste          #+#    #+#             */
/*   Updated: 2022/03/24 15:16:38 by vrogiste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*death_loop(void *args)
{
	t_param	*param = (t_param *)args;

	while (true)
	{
		sem_wait(param->sem_ready);
		
		if (get_time() - param->last_eat > (uint64_t)param->time_to_die)
		{
			sem_post(param->sem_last_eat);
			sem_wait(param->sem_print);
			printf("%" PRId64"%d died\n", get_time(), param->index);
			exit(EXIT_SUCCESS);
		}
		sem_post(param->sem_ready);
	}
	return (NULL);
}

void	process(t_param *param, int index)
{
	pthread_t	thread;
	sem_wait(param->sem_ready);
	sem_post(param->sem_ready);
	usleep(1000000);
	uint64_t	t_zero = get_time();
	sem_wait(param->sem_last_eat);
	param->last_eat = get_time();
	sem_post(param->sem_last_eat);
	pthread_create(&thread, NULL, death_loop, param);
	while (true)
	{
		sem_wait(param->sem_forks);
		sem_wait(param->sem_forks);
		sem_wait(param->sem_print);
		printf("%" PRId64 " %d is eating\n",get_time() - t_zero, index + 1);
		sem_post(param->sem_print);
		micro_sleep(param->time_to_eat);
		sem_post(param->sem_forks);
		sem_post(param->sem_forks);

		sem_wait(param->sem_ready);
		param->last_eat = get_time();
		sem_post(param->sem_ready);

		sem_wait(param->sem_print);
		printf("%" PRId64 " %d is sleeping\n",get_time() - t_zero, index + 1);
		sem_post(param->sem_print);
		micro_sleep(param->time_to_sleep);
		sem_wait(param->sem_print);
		printf("%" PRId64 " %d is thinking\n",get_time() - t_zero, index + 1);
		sem_post(param->sem_print);
	}
	exit(EXIT_SUCCESS);
}
