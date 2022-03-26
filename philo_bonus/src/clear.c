/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clear.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vrogiste <vrogiste@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 12:41:31 by vrogiste          #+#    #+#             */
/*   Updated: 2022/03/26 16:54:24 by vrogiste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	unlink_param(void)
{
	sem_unlink(SEM_READY);
	sem_unlink(SEM_FORKS);
	sem_unlink(SEM_PRINT);
	sem_unlink(SEM_LAST_EAT);
}

static void	lst_delone(t_list *lst)
{
	t_philo	*philo;

	if (lst)
	{
		philo = (t_philo *)lst->content;
		sem_unlink(philo->sem_n_eaten_name);
		free(philo->sem_n_eaten_name);
		free(lst->content);
		free(lst);
	}
}

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
		lst_delone(lst);
		lst = swap;
		i++;
	}
}
