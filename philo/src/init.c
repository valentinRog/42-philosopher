/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vrogiste <vrogiste@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/20 13:48:06 by vrogiste          #+#    #+#             */
/*   Updated: 2022/03/21 20:14:45 by vrogiste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	fill_param(t_param *param, int argc, char **argv)
{
	bool	error;

	error = false;
	if (argc < 5 || argc > 6)
		return (true);
	param->number_of_philo = atoi_error(argv[1], &error);
	if (error)
		return (true);
	param->time_to_die = atoi_error(argv[2], &error);
	if (error)
		return (true);
	param->time_to_eat = atoi_error(argv[3], &error);
	if (error)
		return (true);
	param->time_to_sleep = atoi_error(argv[4], &error);
	if (error)
		return (true);
	if (argc == 6)
		param->number_of_eating = atoi_error(argv[5], &error);
	else
		param->number_of_eating = 10000000;
	param->death = false;
	return (error);
}

bool	init_philo(t_param *param, t_list **alst)
{
	int		i;
	t_philo	*philo;

	i = -1;
	while (++i < param->number_of_philo)
	{
		philo = malloc(sizeof(t_philo));
		if (!philo)
		{
			lst_clear(*alst);
			return (true);
		}
		philo->index = i + 1;
		philo->param = param;
		philo->n_eaten = 0;
		lst_add_back(alst, lst_new(philo));
	}
	return (false);
}

bool	init_mutex(t_list *lst)
{
	t_list	*node;
	t_param	*param;
	size_t	i;
	t_philo	*philo;

	node = lst;
	param = ((t_philo *)node->content)->param;
	if (pthread_mutex_init(&param->mutex_ready, NULL))
		return (true);
	if (pthread_mutex_init(&param->mutex_death, NULL))
		return (true);
	if (pthread_mutex_init(&param->mutex_print, NULL))
		return (true);
	i = 0;
	while (i < lst_size(lst))
	{
		philo = (t_philo *)node->content;
		if (pthread_mutex_init(&philo->mutex_fork, NULL))
			return (true);
		if (pthread_mutex_init(&philo->mutex_last_eat, NULL))
			return (true);
		node = node->next;
		i++;
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
	micro_sleep(param->time_to_die);
	return (false);
}
