/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vrogiste <vrogiste@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/11 16:45:39 by vrogiste          #+#    #+#             */
/*   Updated: 2022/03/20 14:43:10 by vrogiste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	monitor(t_philo *philo, int action)
{
	if (check_death(philo))
		return ;
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

int	main(int argc, char **argv)
{
	t_param		param;
	t_list		*lst;

	lst = NULL;
	if (fill_param(&param, argc, argv))
		return (1);
	init_philo(&param, &lst);
	init_mutex(lst);
	init_threads(lst);
	return (0);
}
