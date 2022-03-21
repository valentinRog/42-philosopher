/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clear.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vrogiste <vrogiste@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 19:01:01 by vrogiste          #+#    #+#             */
/*   Updated: 2022/03/21 21:57:29 by vrogiste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	lst_clear(t_list *lst)
{
	t_list	*swap;
	size_t	i;
	size_t	len;
	t_philo	*philo;

	len = lst_size(lst);
	i = 0;
	while (i < len)
	{
		philo = (t_philo *)lst->content;
		swap = lst->next;
		pthread_mutex_destroy(&philo->mutex_fork);
		pthread_mutex_destroy(&philo->mutex_last_eat);
		free(lst->content);
		free(lst);
		lst = swap;
		i++;
	}
}

void	destroy_param_mutex(t_param *param)
{
	pthread_mutex_destroy(&param->mutex_ready);
	pthread_mutex_destroy(&param->mutex_death);
	pthread_mutex_destroy(&param->mutex_print);
}
