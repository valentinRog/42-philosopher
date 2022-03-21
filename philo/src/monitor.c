/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vrogiste <vrogiste@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 20:16:29 by vrogiste          #+#    #+#             */
/*   Updated: 2022/03/21 20:20:37 by vrogiste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	monitor(t_philo *philo, int action)
{
	pthread_mutex_lock(&philo->param->mutex_print);
	if (check_death(philo))
	{
		pthread_mutex_unlock(&philo->param->mutex_print);
		return ;
	}
	printf("%ld %d", get_time() - philo->param->time_zero, philo->index);
	if (action == FORK)
		printf(" has taken a fork\n");
	else if (action == EAT)
		printf(" is eating\n");
	else if (action == SLEEP)
		printf(" is sleeping\n");
	else if (action == THINK)
		printf(" is thinking\n");
	else if (action == DIE)
		printf(" died\n");
	pthread_mutex_unlock(&philo->param->mutex_print);
}

void	death_loop(t_list *lst)
{
	t_list	*node;
	t_philo	*philo;

	node = lst;
	while (true)
	{
		philo = (t_philo *)node->content;
		pthread_mutex_lock(&philo->mutex_last_eat);
		if (get_time() - philo->last_eat >= (uint64_t)philo->param->time_to_die)
		{
			pthread_mutex_unlock(&philo->mutex_last_eat);
			monitor(philo, DIE);
			pthread_mutex_lock(&philo->param->mutex_death);
			philo->param->death = true;
			pthread_mutex_unlock(&philo->param->mutex_death);
			break ;
		}
		pthread_mutex_unlock(&philo->mutex_last_eat);
		node = node->next;
		usleep(30);
	}
}

void	join_philos(t_list *lst)
{
	t_list	*node;
	size_t	i;

	node = lst;
	i = 0;
	while (i < lst_size(lst))
	{
		pthread_join(((t_philo *)node->content)->thread, NULL);
		node = node->next;
		i++;
	}
}
