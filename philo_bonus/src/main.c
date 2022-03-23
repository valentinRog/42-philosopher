/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vrogiste <vrogiste@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/11 16:45:39 by vrogiste          #+#    #+#             */
/*   Updated: 2022/03/23 18:25:38 by vrogiste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_param	param;
	sem_unlink("yo");
	sem_unlink("ready");
	fill_param(&param, argc, argv);
	sem_t *yo = sem_open("yo", O_CREAT, 0660, param.number_of_philo);
	sem_t *ready = sem_open("ready", O_CREAT, 0660, 1);
	sem_wait(ready);
	for (int i = 0; i < param.number_of_philo; i++)
	{
		if (!fork())
		{
			sem_wait(ready);
			sem_post(ready);
			uint64_t	t_zero = get_time();
			while (true)
			{
				sem_wait(yo);
				sem_wait(yo);
					printf("%" PRId64 " %d is eating\n",get_time() - t_zero, i + 1);
					micro_sleep(param.time_to_eat);
				sem_post(yo);
				sem_post(yo);
				printf("%" PRId64 " %d is sleeping\n",get_time() - t_zero, i + 1);
				micro_sleep(param.time_to_sleep);
				printf("%" PRId64 " %d is thinking\n",get_time() - t_zero, i + 1);
			}
			exit(EXIT_SUCCESS);
		}
	}
	sem_post(ready);
	wait(NULL);
	sem_close(yo);
	sem_close(ready);
	return (0);
}
