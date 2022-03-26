/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launcher.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vrogiste <vrogiste@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/26 22:34:24 by vrogiste          #+#    #+#             */
/*   Updated: 2022/03/26 22:51:58 by vrogiste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	eat_enough(t_list *lst)
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

static void	set_death(t_param *param)
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
			printf("%" PRIu64, get_time() - philo->param->time_zero);
			printf(" %d died\n", philo->index + 1);
			break ;
		}
		pthread_mutex_unlock(&philo->mutex_last_eat);
		node = node->next;
	}
}

bool	launch_threads(t_list *lst)
{
	t_list		*node;
	t_param		*param;
	size_t		i;
	pthread_t	*thread;

	node = lst;
	param = ((t_philo *)node->content)->param;
	pthread_mutex_lock(&param->mutex_ready);
	i = 0;
	while (i < lst_size(lst))
	{
		thread = &((t_philo *)node->content)->thread;
		if (pthread_create(thread, NULL, philoop, node))
			return (true);
		node = node->next;
		i++;
	}
	param->time_zero = get_time();
	pthread_mutex_unlock(&param->mutex_ready);
	return (false);
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
