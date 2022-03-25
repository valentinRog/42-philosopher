/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vrogiste <vrogiste@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/20 13:48:06 by vrogiste          #+#    #+#             */
/*   Updated: 2022/03/25 13:26:01 by vrogiste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	init_sem(t_param *param)
{
	unlink_param();
	param->sem_ready = sem_open(SEM_READY, O_CREAT, 0660, 1);
	param->sem_forks = sem_open(SEM_FORKS, O_CREAT, 0660, param->number_of_philo);
	param->sem_print = sem_open(SEM_PRINT, O_CREAT, 0660, 1);
	param->sem_last_eat = sem_open(SEM_LAST_EAT, O_CREAT, 0660, 1);
	return (false);
}

bool	init_param(t_param *param, int argc, char **argv)
{
	if (argc < 5 || argc > 6)
		return (true);
	param->number_of_eating = 10000000;
	if (atoi_error(argv[1], &param->number_of_philo))
		return (true);
	if (atoi_error(argv[2], &param->time_to_die))
		return (true);
	if (atoi_error(argv[3], &param->time_to_eat))
		return (true);
	if (atoi_error(argv[4], &param->time_to_sleep))
		return (true);
	if (argc == 6)
		if (atoi_error(argv[5], &param->number_of_eating))
			return (true);
	if (init_sem(param))
		return (true);
	return (false);
}

bool	init_philo(t_param *param, t_list **alst)
{
	int		i;
	t_philo	*philo;
	t_list	*new_node;

	i = 0;
	while (i < param->number_of_philo)
	{
		philo = malloc(sizeof(t_philo));
		if (!philo)
			return (true);
		philo->index = i;
		philo->param = param;
		philo->sem_n_eaten_name = ft_itoa(i);
		sem_unlink(philo->sem_n_eaten_name);
		philo->sem_n_eaten = sem_open(philo->sem_n_eaten_name, O_CREAT, 0660, param->number_of_eating);
		new_node = lst_new(philo);
		if (!new_node)
		{
			free(philo);
			return (true);
		}
		lst_add_back(alst, new_node);
		i++;
	}
	return (false);
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
	return (false);
}