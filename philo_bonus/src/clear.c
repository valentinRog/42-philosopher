/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clear.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vrogiste <vrogiste@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 12:41:31 by vrogiste          #+#    #+#             */
/*   Updated: 2022/03/25 13:10:40 by vrogiste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	unlink_param()
{
	sem_unlink(SEM_READY);
	sem_unlink(SEM_FORKS);
	sem_unlink(SEM_PRINT);
	sem_unlink(SEM_LAST_EAT);
}

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
		sem_unlink(philo->sem_n_eaten_name);
		free(philo->sem_n_eaten_name);
		free(lst->content);
		free(lst);
		lst = swap;
		i++;
	}
}