/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vrogiste <vrogiste@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 09:57:52 by vrogiste          #+#    #+#             */
/*   Updated: 2022/03/26 22:12:13 by vrogiste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	monitor(t_philo *philo, int action)
{
	sem_wait(philo->param->sem_print);
	printf("%" PRIu64, get_time() - philo->param->time_zero);
	printf(" %d", philo->index + 1);
	if (action == FORK)
		printf(" has taken a fork\n");
	else if (action == EAT)
		printf(" is eating\n");
	else if (action == SLEEP)
		printf(" is sleeping\n");
	else if (action == THINK)
		printf(" is thinking\n");
	else if (action == FORK)
		printf(" has taken a fork\n");
	sem_post(philo->param->sem_print);
}

void	*death_loop(void *args)
{
	t_philo	*philo;
	t_param	*param;

	philo = (t_philo *)args;
	param = philo->param;
	while (true)
	{
		sem_wait(param->sem_last_eat);
		if (get_time() - param->last_eat > (uint64_t)param->time_to_die)
		{
			sem_post(param->sem_last_eat);
			sem_wait(param->sem_print);
			printf("%" PRId64, get_time() - param->time_zero);
			printf(" %d died\n", philo->index);
			exit(EXIT_SUCCESS);
		}
		sem_post(param->sem_last_eat);
	}
	return (NULL);
}

static void	eat(t_philo *philo)
{
	sem_wait(philo->param->sem_forks);
	monitor(philo, FORK);
	sem_wait(philo->param->sem_forks);
	monitor(philo, FORK);
	monitor(philo, EAT);
	micro_sleep(philo->param->time_to_eat);
	sem_post(philo->param->sem_forks);
	sem_post(philo->param->sem_forks);
	sem_wait(philo->param->sem_last_eat);
	philo->param->last_eat = get_time();
	sem_post(philo->param->sem_last_eat);
	philo->n_eaten++;
	if (philo->n_eaten >= philo->param->number_of_eating)
		sem_post(philo->sem_n_eaten);
}

void	process(t_philo *philo)
{
	pthread_t	thread;

	sem_wait(philo->sem_n_eaten);
	sem_wait(philo->param->sem_ready);
	sem_post(philo->param->sem_ready);
	philo->param->time_zero = get_time();
	philo->param->last_eat = get_time();
	if (pthread_create(&thread, NULL, death_loop, philo))
		exit(EXIT_FAILURE);
	if (philo->index >= philo->param->n_philo / 2)
		micro_sleep(philo->param->time_to_eat / 3);
	while (true)
	{
		eat(philo);
		monitor(philo, SLEEP);
		micro_sleep(philo->param->time_to_sleep);
		monitor(philo, THINK);
	}
}
