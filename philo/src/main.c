/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/04 16:43:38 by root              #+#    #+#             */
/*   Updated: 2022/09/04 18:40:38 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	init_game(t_game *g, int argc, char **argv)
{
	int		i;
	bool	err;

	memset(g, 0, sizeof(t_game));
	if (argc < 5 || argc > 6)
		return (true);
	g->n = ft_atoi(argv[1]);
	g->time_to_die = ft_atoi(argv[2]);
	g->time_to_eat = ft_atoi(argv[3]);
	g->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		g->n_eat = ft_atoi(argv[5]);
	err = (g->n <= 0) | (g->time_to_die < 0) | (g->time_to_eat < 0)
		| (g->time_to_sleep < 0) | (g->n_eat < 0);
	i = 0;
	g->running = true;
	err |= pthread_mutex_init(&g->print_lock, NULL);
	err |= pthread_mutex_init(&g->start_lock, NULL);
	while (i < g->n)
	{
		g->philos[i].i = i;
		err |= pthread_mutex_init(g->forks + i, NULL);
		i++;
	}
	return (err);
}

static bool	init_threads(t_game *g)
{
	static t_arg	args[PHILO_MAX];
	int				i;
	bool			err;

	i = 0;
	err = false;
	pthread_mutex_lock(&g->start_lock);
	while (i < g->n)
	{
		args[i].content[0] = g->philos + i;
		args[i].content[1] = g;
		g->philos[i].last_eat = get_time();
		err |= pthread_create(g->threads + i, NULL, philoop, args + i);
		i++;
	}
	g->t0 = get_time();
	pthread_mutex_unlock(&g->start_lock);
	return (err);
}

static void	death_loop(t_game *g)
{
	int		i;
	t_philo	*p;
	int		stop;

	i = 0;
	while (g->running)
	{
		p = g->philos + i;
		stop = get_time() - p->last_eat > g->time_to_die;
		stop |= (g->n == g->ate_enough) << 1;
		if (stop)
		{
			pthread_mutex_lock(&g->print_lock);
			g->running = false;
			if (stop & 1)
				printf("%d %d died\n", get_time() - g->t0, p->i + 1);
			pthread_mutex_unlock(&g->print_lock);
		}
		i++;
		i %= g->n;
	}
}

static void	join_threads(t_game *g)
{
	int		i;

	i = 0;
	while (i < g->n)
	{
		pthread_join(g->threads[i], NULL);
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_game	g;

	if (init_game(&g, argc, argv))
		return (EXIT_FAILURE);
	if (init_threads(&g))
		return (EXIT_FAILURE);
	death_loop(&g);
	join_threads(&g);
}
