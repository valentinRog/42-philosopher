/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vrogiste <vrogiste@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/11 16:45:39 by vrogiste          #+#    #+#             */
/*   Updated: 2022/03/26 22:51:58 by vrogiste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_param		param;
	t_list		*lst;

	lst = NULL;
	if (fill_param(&param, argc, argv))
		return (1);
	if (init_philo(&param, &lst) || launch_threads(lst))
	{
		destroy_param_mutex(&param);
		lst_clear(lst);
		return (1);
	}
	micro_sleep(param.time_to_die);
	death_loop(lst);
	join_philos(lst);
	destroy_param_mutex(&param);
	lst_clear(lst);
	return (0);
}
