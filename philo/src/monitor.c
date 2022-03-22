/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vrogiste <vrogiste@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 20:16:29 by vrogiste          #+#    #+#             */
/*   Updated: 2022/03/22 18:27:37 by vrogiste         ###   ########.fr       */
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
	printf("%llu %d", get_time() - philo->param->time_zero, philo->index);
	if (action == FORK)
		printf(" has taken a fork\n");
	else if (action == EAT)
		printf(" is eating\n");
	else if (action == SLEEP)
		printf(" is sleeping\n");
	else if (action == THINK)
		printf(" is thinking\n");
	pthread_mutex_unlock(&philo->param->mutex_print);
}

bool	eat_enough(t_list *lst)
{
	size_t	i;
	t_philo	*philo;

	i = 0;
	while (i < lst_size(lst))
	{
		philo = (t_philo *)lst->content;
		pthread_mutex_lock(&philo->mutex_n_eaten);
		if (philo->n_eaten < philo->param->number_of_eating)
		{
			pthread_mutex_unlock(&philo->mutex_n_eaten);
			return (false);
		}
		pthread_mutex_unlock(&philo->mutex_n_eaten);
		lst = lst->next;
		i++;
	}
	return (true);
}

void	set_death(t_param *param)
{
	pthread_mutex_lock(&param->mutex_death);
	param->death = true;
	pthread_mutex_unlock(&param->mutex_death);
}

void	death_loop(t_list *lst)
{
	t_list	*node;
	t_philo	*philo;

	node = lst;
	while (true)
	{
		philo = (t_philo *)node->content;
		if (eat_enough(lst))
		{
			set_death(philo->param);
			break ;
		}
		pthread_mutex_lock(&philo->mutex_last_eat);
		if (get_time() - philo->last_eat > (uint64_t)philo->param->time_to_die)
		{
			set_death(philo->param);
			pthread_mutex_unlock(&philo->mutex_last_eat);
			printf("%llu", get_time() - philo->param->time_zero);
			printf(" %d died\n", philo->index);
			break ;
		}
		pthread_mutex_unlock(&philo->mutex_last_eat);
		node = node->next;
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
