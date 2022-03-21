/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clear.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vrogiste <vrogiste@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 19:01:01 by vrogiste          #+#    #+#             */
/*   Updated: 2022/03/21 21:21:39 by vrogiste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	lst_clear(t_list *lst)
{
	t_list	*swap;
	size_t	i;
	size_t	len;

	len = lst_size(lst);
	i = 0;
	while (i < len)
	{
		swap = lst->next;
		free(lst->content);
		free(lst);
		lst = swap;
		i++;
	}
}

void	destroy_mutex(t_list *lst)
{
	t_philo	*philo;
	size_t	i;

	if (!lst)
		return ;
	philo = (t_philo *)lst->content;
	pthread_mutex_destroy(&philo->param->mutex_ready);
	pthread_mutex_destroy(&philo->param->mutex_death);
	pthread_mutex_destroy(&philo->param->mutex_print);
	i = 0;
	while (i < lst_size(lst))
	{
		pthread_mutex_destroy(&philo->mutex_fork);
		pthread_mutex_destroy(&philo->mutex_last_eat);
		philo = (t_philo *) lst->content;
		lst = lst->next;
		i++;
	}
}
