/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launcher.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vrogiste <vrogiste@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 16:16:55 by vrogiste          #+#    #+#             */
/*   Updated: 2022/03/27 09:48:56 by vrogiste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	kill_all(t_list *lst)
{
	t_list	*node;
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

static int	fork_safe(t_list *lst)
{
	int	pid;

	pid = fork();
	if (pid < 0)
	{
		kill_all(lst);
		lst_clear(lst);
		exit(EXIT_FAILURE);
	}
	return (pid);
}

static void	watch_process(t_list *lst)
{
	if (!fork_safe(lst))
	{
		watch_n_eaten(lst);
		exit(EXIT_SUCCESS);
	}
	sem_post(((t_philo *)lst->content)->param->sem_ready);
	wait(NULL);
	kill_all(lst);
}

void	launch_process(t_list *lst)
{
	t_philo		*philo;
	t_param		*param;
	t_list		*node;
	size_t		i;

	param = ((t_philo *)lst->content)->param;
	node = lst;
	sem_wait(param->sem_ready);
	i = 0;
	while (i < lst_size(lst))
	{
		philo = (t_philo *)node->content;
		philo->pid = fork_safe(lst);
		if (!philo->pid)
		{
			process(node);
			exit(EXIT_SUCCESS);
		}
		node = node->next;
		i++;
	}
	watch_process(lst);
}
