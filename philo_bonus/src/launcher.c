/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launcher.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vrogiste <vrogiste@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 16:16:55 by vrogiste          #+#    #+#             */
/*   Updated: 2022/03/25 16:17:27 by vrogiste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void kill_all(t_list *lst)
{
	t_list	*node = lst;
	t_philo	*philo;
	for (int i = 0; i < lst_size(lst); i++)
	{
		philo = (t_philo *)node->content;
		kill(philo->pid, SIGKILL);
		node = node->next;
	}
}

bool	init_process(t_list *lst)
{
	t_philo	*philo;
	t_param	*param;
	t_list	*node;

	param = ((t_philo *)lst->content)->param;
	node = lst;
	sem_wait(param->sem_ready);
	for (int i = 0; i < lst_size(lst); i++)
	{
		philo = (t_philo *)node->content;
		philo->pid = fork();
		if (!philo->pid)
		{
			process(philo);
			exit(EXIT_SUCCESS);
		}
		node = node->next;
	}
	sem_post(param->sem_ready);
	wait(NULL);
	kill_all(lst);
	return (false);
}