/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vrogiste <vrogiste@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/20 13:50:22 by vrogiste          #+#    #+#             */
/*   Updated: 2022/03/26 22:39:43 by vrogiste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	check_death(t_philo *philo)
{
	bool	death;

	pthread_mutex_lock(&philo->param->mutex_death);
	death = philo->param->death;
	pthread_mutex_unlock(&philo->param->mutex_death);
	return (death);
}

static void	monitor(t_philo *philo, int action)
{
	pthread_mutex_lock(&philo->param->mutex_print);
	if (check_death(philo))
	{
		pthread_mutex_unlock(&philo->param->mutex_print);
		return ;
	}
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
	pthread_mutex_unlock(&philo->param->mutex_print);
}

static void	eat(t_philo *philo, t_philo *next_philo)
{
	if (philo->index > next_philo->index)
	{
		pthread_mutex_lock(&philo->mutex_fork);
		monitor(philo, FORK);
		pthread_mutex_lock(&next_philo->mutex_fork);
		monitor(philo, FORK);
	}
	else
	{
		pthread_mutex_lock(&next_philo->mutex_fork);
		monitor(philo, FORK);
		pthread_mutex_lock(&philo->mutex_fork);
		monitor(philo, FORK);
	}
	monitor(philo, EAT);
	micro_sleep(philo->param->time_to_eat);
	pthread_mutex_unlock(&philo->mutex_fork);
	pthread_mutex_unlock(&next_philo->mutex_fork);
	pthread_mutex_lock(&philo->mutex_last_eat);
	philo->last_eat = get_time();
	pthread_mutex_unlock(&philo->mutex_last_eat);
	pthread_mutex_lock(&philo->mutex_n_eaten);
	philo->n_eaten++;
	pthread_mutex_unlock(&philo->mutex_n_eaten);
}

void	*philoop(void *arg)
{
	t_list	*node;
	t_philo	*philo;
	t_philo	*next_philo;

	node = (t_list *)arg;
	philo = (t_philo *)node->content;
	next_philo = (t_philo *)node->next->content;
	pthread_mutex_lock(&philo->param->mutex_ready);
	pthread_mutex_unlock(&philo->param->mutex_ready);
	pthread_mutex_lock(&philo->mutex_last_eat);
	philo->last_eat = get_time();
	pthread_mutex_unlock(&philo->mutex_last_eat);
	if (philo->index % 2)
		micro_sleep(philo->param->time_to_eat / 2);
	while (!check_death(philo))
	{
		if (philo != next_philo)
		{
			eat(philo, next_philo);
			monitor(philo, SLEEP);
			micro_sleep(philo->param->time_to_sleep);
			monitor(philo, THINK);
		}
	}
	return (NULL);
}
