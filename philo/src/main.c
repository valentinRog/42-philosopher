/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vrogiste <vrogiste@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/11 16:45:39 by vrogiste          #+#    #+#             */
/*   Updated: 2022/03/21 20:12:29 by vrogiste         ###   ########.fr       */
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

int	main(int argc, char **argv)
{
	t_param		param;
	t_list		*lst;

	lst = NULL;
	if (fill_param(&param, argc, argv))
		return (1);
	if (init_philo(&param, &lst))
		return (1);
	if (init_mutex(lst))
		return (1);
	if (init_threads(lst))
		return (1);
	death_loop(lst);
	t_list *node = lst;
	for (int i = 0; i < (int)lst_size(lst); i++)
	{
		pthread_join(((t_philo *)node->content)->thread, NULL);
		node = node->next;
	}
	lst_clear(lst);
	return (0);
}
