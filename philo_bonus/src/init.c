/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vrogiste <vrogiste@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/20 13:48:06 by vrogiste          #+#    #+#             */
/*   Updated: 2022/03/24 15:23:37 by vrogiste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	init_sem(t_param *param)
{
	sem_unlink(SEM_READY);
	sem_unlink(SEM_FORKS);
	sem_unlink(SEM_PRINT);
	sem_unlink(SEM_LAST_EAT);
	param->sem_ready = sem_open(SEM_READY, O_CREAT, 0660, 1);
	param->sem_forks = sem_open(SEM_FORKS, O_CREAT, 0660, param->number_of_philo);
	param->sem_print = sem_open(SEM_PRINT, O_CREAT, 0660, 1);
	param->sem_last_eat = sem_open(SEM_LAST_EAT, O_CREAT, 0660, 1);
	return (false);
}

bool	fill_param(t_param *param, int argc, char **argv)
{
	if (argc < 5 || argc > 6)
		return (true);
	param->number_of_eating = 10000000;
	if (atoi_error(argv[1], &param->number_of_philo))
		return (true);
	if (atoi_error(argv[2], &param->time_to_die))
		return (true);
	if (atoi_error(argv[3], &param->time_to_eat))
		return (true);
	if (atoi_error(argv[4], &param->time_to_sleep))
		return (true);
	if (argc == 6)
		if (atoi_error(argv[5], &param->number_of_eating))
			return (true);
	if (init_sem(param))
		return (true);
	return (false);
}

bool	init_process(t_param *param)
{
	int	pid_arr[PHILO_MAX + 1];

	sem_wait(param->sem_ready);
	for (int i = 0; i < param->number_of_philo; i++)
	{
		param->index = i;
		pid_arr[i] = fork();
		if (!pid_arr[i])
			process(param, i);
	}
	sem_post(param->sem_ready);
	wait(NULL);
	for (int i = 0; i < PHILO_MAX; i++)
		kill(pid_arr[i], SIGCHLD);
	return (false);
}
