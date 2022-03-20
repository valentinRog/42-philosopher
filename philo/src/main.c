/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vrogiste <vrogiste@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/11 16:45:39 by vrogiste          #+#    #+#             */
/*   Updated: 2022/03/20 12:02:35 by vrogiste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

uint64_t	get_time(void)
{
	static struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * (uint64_t)1000) + (tv.tv_usec / 1000));
}

void	monitor(t_philo *philo, int action)
{
	pthread_mutex_lock(&philo->param->mutex_print);
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

void	smart_sleep(uint64_t time)
{
	uint64_t i = get_time();
	while (true)
	{
		if (get_time() - i >= time)
			break;
		usleep(50);
	}
}

void	*philoop(void *arg)
{
	t_list	*node = (t_list *)arg;
	t_philo	*philo = (t_philo*)node->content;
	t_philo	*r_philo = (t_philo*)node->next->content;

	while (true)
	{
		pthread_mutex_lock(&philo->param->mutex_ready);
		if (philo->param->threads_ready)
		{
			pthread_mutex_unlock(&philo->param->mutex_ready);
			break;
		}
		pthread_mutex_unlock(&philo->param->mutex_ready);
		usleep(50);
	}
	pthread_mutex_lock(&philo->mutex_last_eat);
	philo->last_eat = get_time();
	pthread_mutex_unlock(&philo->mutex_last_eat);
	while (true)
	{
		if (philo->index < r_philo->index)
		{
			pthread_mutex_lock(&philo->mutex_fork);
			pthread_mutex_lock(&r_philo->mutex_fork);
		}
		else
		{
			pthread_mutex_lock(&r_philo->mutex_fork);
			pthread_mutex_lock(&philo->mutex_fork);
		}
		monitor(philo, EAT);
		smart_sleep(philo->param->time_to_eat);
		pthread_mutex_lock(&philo->mutex_last_eat);
		philo->last_eat = get_time();
		pthread_mutex_unlock(&philo->mutex_last_eat);
		pthread_mutex_unlock(&philo->mutex_fork);
		pthread_mutex_unlock(&r_philo->mutex_fork);
		monitor(philo, SLEEP);
		smart_sleep(philo->param->time_to_sleep);
		monitor(philo, THINK);
	}
	return NULL;
}

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
	param->death = false;
	param->threads_ready = false;
	return (error);
}

void	init_philo(t_param *param, t_list **alst)
{
	for (int i = 0; i < param->number_of_philo; i++)
	{
		t_philo	*philo = malloc(sizeof(t_philo));
		philo->index = i + 1;
		philo->param = param;
		lst_add_back(alst, lst_new(philo));
	}
	
}

void	init_mutex(t_list *lst)
{
	t_list	*node = lst;
	t_param	*param = ((t_philo *)node->content)->param;

	pthread_mutex_init(&param->mutex_ready, NULL);
	pthread_mutex_init(&param->mutex_death, NULL);
	pthread_mutex_init(&param->mutex_print, NULL);
	for (int i = 0; i < (int)lst_size(lst); i++)
	{
		pthread_mutex_init(&((t_philo *)node->content)->mutex_fork, NULL);
		pthread_mutex_init(&((t_philo *)node->content)->mutex_last_eat, NULL);
		node = node->next;
	}
}

void	init_threads(t_list *lst)
{
	t_list	*node = lst;
	t_param	*param = ((t_philo *)node->content)->param;
	for (int i = 0; i < (int)lst_size(lst); i++)
	{
		pthread_create(&((t_philo *)node->content)->thread, NULL, philoop, node);
		node = node->next;
	}
	pthread_mutex_lock(&param->mutex_ready);
	param->threads_ready = true;
	param->time_zero = get_time();
	pthread_mutex_unlock(&param->mutex_ready);
	node = lst;
	for (int i = 0; i < (int)lst_size(lst); i++)
	{
		pthread_join(((t_philo *)node->content)->thread, NULL);
		node = node->next;
	}
}

int	main(int argc, char **argv)
{
	t_param		param;
	t_list		*lst = NULL;

	if (fill_param(&param, argc, argv))
		return (1);
	init_philo(&param, &lst);
	init_mutex(lst);
	init_threads(lst);
	return (0);
}
