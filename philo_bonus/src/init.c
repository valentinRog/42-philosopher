/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vrogiste <vrogiste@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/20 13:48:06 by vrogiste          #+#    #+#             */
/*   Updated: 2022/03/25 16:30:36 by vrogiste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	init_param_sem(t_param *param)
{
	unlink_param();
	param->sem_ready = sem_open(SEM_READY, O_CREAT, 0660, 1);
	param->sem_forks = sem_open(SEM_FORKS, O_CREAT, 0660, param->number_of_philo);
	param->sem_print = sem_open(SEM_PRINT, O_CREAT, 0660, 1);
	param->sem_last_eat = sem_open(SEM_LAST_EAT, O_CREAT, 0660, 1);
	if (param->sem_ready == SEM_FAILED || param->sem_forks == SEM_FAILED)
		return (true);
	if (param->sem_print == SEM_FAILED || param->sem_last_eat == SEM_FAILED)
		return (true);
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
	if (init_param_sem(param))
		return (true);
	return (false);
}

static t_philo	*new_philo(t_param *param, int index)
{
	t_philo	*philo;

	philo = malloc(sizeof(t_philo));
	if (!philo)
		return (NULL);
	philo->index = index;
	philo->param = param;
	philo->sem_n_eaten_name = ft_itoa(index);
	if (!philo->sem_n_eaten_name)
	{
		free(philo);
		return (NULL);
	}
	philo->n_eaten = 0;
	philo->pid = -1;
	sem_unlink(philo->sem_n_eaten_name);
	philo->sem_n_eaten = sem_open(philo->sem_n_eaten_name, O_CREAT, 0660, 1);
	return (philo);
}

bool	init_philo(t_param *param, t_list **alst)
{
	int		i;
	t_list	*new_node;

	i = 0;
	while (i < param->number_of_philo)
	{
		new_node = lst_new(new_philo(param, i));
		if (!new_node || !new_node->content)
		{
			lst_clear(*alst);
			return (true);
		}
		lst_add_back(alst, new_node);
		i++;
	}
	return (false);
}