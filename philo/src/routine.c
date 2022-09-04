/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/04 17:18:53 by root              #+#    #+#             */
/*   Updated: 2022/09/04 18:48:15 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	monitor(t_philo *p, t_game *g, char *action)
{
	pthread_mutex_lock(&g->print_lock);
	if (g->running)
		printf("%d %d %s\n", get_time() - g->t0, p->i + 1, action);
	pthread_mutex_unlock(&g->print_lock);
}

static void	pick_forks(t_philo *p, t_game *g)
{
	if (g->n % 2 && p->i == g->n - 1)
	{
		while (g->turn != 2)
			if (!g->running)
				return ;
	}
	else
	{
		while (g->turn != p->i % 2)
			if (!g->running)
				return ;
	}
	pthread_mutex_lock(g->forks + p->i);
	monitor(p, g, FORK);
	pthread_mutex_lock(g->forks + (p->i + 1) % g->n);
	monitor(p, g, FORK);
	g->ready_to_eat++;
	if (g->ready_to_eat == g->n / 2 || g->turn == 2)
	{
		g->ready_to_eat = 0;
		g->turn++;
		g->turn %= 3;
		if (g->turn == 2 && !(g->n % 2))
			g->turn = 0;
	}
}

static void	eat(t_philo *p, t_game *g)
{
	pick_forks(p, g);
	monitor(p, g, EAT);
	milli_sleep(g->time_to_eat);
	pthread_mutex_unlock(g->forks + p->i);
	pthread_mutex_unlock(g->forks + (p->i + 1) % g->n);
	p->last_eat = get_time();
	p->n_eaten++;
	if (p->n_eaten == g->n_eat)
		g->ate_enough++;
}

void	*philoop(void *arg)
{
	t_philo	*p;
	t_game	*g;

	p = ((t_arg *)arg)->content[0];
	g = ((t_arg *)arg)->content[1];
	pthread_mutex_lock(&g->start_lock);
	pthread_mutex_unlock(&g->start_lock);
	while (g->running)
	{
		eat(p, g);
		monitor(p, g, SLEEP);
		if (g->running)
			milli_sleep(g->time_to_sleep);
		monitor(p, g, THINK);
	}
	return (NULL);
}
