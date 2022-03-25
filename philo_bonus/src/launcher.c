/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launcher.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vrogiste <vrogiste@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 16:16:55 by vrogiste          #+#    #+#             */
/*   Updated: 2022/03/25 16:59:49 by vrogiste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	kill_all(t_list *lst)
{
	t_list	*node = lst;
	t_philo	*philo;
	size_t	i;

	node = lst;
	i = 0;
	while (i < lst_size(lst))
	{
		philo = (t_philo *)node->content;
		kill(philo->pid, SIGKILL);
		node = node->next;
		i++;
	}
}

static void	*watch_n_eaten(void *args)
{
	t_list	*lst;
	t_list	*node;
	size_t	i;
	t_philo	*philo;

	lst = (t_list *)args;
	node = lst;
	i = 0;
	while (i < lst_size(lst))
	{
		philo = (t_philo *)node->content;
		sem_wait(philo->sem_n_eaten);
		node = node->next;
		i++;
	}
	return (NULL);	
}

bool	init_process(t_list *lst)
{
	t_philo		*philo;
	t_param		*param;
	t_list		*node;

	param = ((t_philo *)lst->content)->param;
	node = lst;
	sem_wait(param->sem_ready);
	for (int i = 0; i < lst_size(lst); i++)
	{
		philo = (t_philo *)node->content;
		philo->pid = fork();
		if (!philo->pid)
			process(philo);
		else if (philo->pid < 0)
			exit(EXIT_FAILURE);
		node = node->next;
	}
	sem_post(param->sem_ready);
	sleep(1);
	if (!fork())
	{
		watch_n_eaten(lst);
		exit(EXIT_SUCCESS);
	}
	wait(NULL);
	kill_all(lst);
	return (false);
}