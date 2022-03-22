/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vrogiste <vrogiste@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/20 13:48:06 by vrogiste          #+#    #+#             */
/*   Updated: 2022/03/22 16:02:23 by vrogiste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	init_param_mutex(t_param *param)
{
	if (pthread_mutex_init(&param->mutex_ready, NULL))
		return (true);
	if (pthread_mutex_init(&param->mutex_death, NULL))
	{
		pthread_mutex_destroy(&param->mutex_ready);
		return (true);
	}
	if (pthread_mutex_init(&param->mutex_print, NULL))
	{
		pthread_mutex_destroy(&param->mutex_ready);
		pthread_mutex_destroy(&param->mutex_death);
		return (true);
	}
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
	if (init_param_mutex(param))
		return (true);
	param->death = false;
	return (false);
}

bool	init_philo_mutex(t_philo *philo)
{
	if (pthread_mutex_init(&philo->mutex_fork, NULL))
		return (true);
	if (pthread_mutex_init(&philo->mutex_last_eat, NULL))
	{
		pthread_mutex_destroy(&philo->mutex_fork);
		return (true);
	}
	if (pthread_mutex_init(&philo->mutex_n_eaten, NULL))
	{
		pthread_mutex_destroy(&philo->mutex_fork);
		pthread_mutex_destroy(&philo->mutex_last_eat);
		return (true);
	}
	return (false);
}

bool	init_philo(t_param *param, t_list **alst)
{
	int		i;
	t_philo	*philo;
	t_list	*new_node;

	i = -1;
	while (++i < param->number_of_philo)
	{
		philo = malloc(sizeof(t_philo));
		if (!philo)
			return (true);
		philo->index = i + 1;
		philo->param = param;
		philo->n_eaten = 0;
		new_node = lst_new(philo);
		if (!new_node || init_philo_mutex(philo))
		{
			free(philo);
			return (true);
		}
		lst_add_back(alst, new_node);
	}
	return (false);
}

bool	init_threads(t_list *lst)
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
