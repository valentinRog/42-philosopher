/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/04 17:18:53 by root              #+#    #+#             */
/*   Updated: 2022/09/04 20:56:51 by root             ###   ########.fr       */
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

static bool	pick_forks(atomic_int *turn, t_philo *p, t_game *g)
{
	if (g->n % 2 && p->i == g->n - 1)
		while (*turn != 2 && g->running)
			usleep(50);
	else
		while (*turn != p->i % 2 && g->running)
			usleep(50);
	if (!g->running)
		return (true);
	if (p->i < (p->i + 1) % g->n)
	{
		pthread_mutex_lock(g->forks + p->i);
		monitor(p, g, FORK);
		pthread_mutex_lock(g->forks + (p->i + 1) % g->n);
		monitor(p, g, FORK);
	}
	else
	{
		pthread_mutex_lock(g->forks + (p->i + 1) % g->n);
		monitor(p, g, FORK);
		pthread_mutex_lock(g->forks + p->i);
		monitor(p, g, FORK);
	}
	return (false);
}

static void	eat(t_philo *p, t_game *g)
{
	static atomic_int	ready_to_eat;
	static atomic_int	turn;

	if (!pick_forks(&turn, p, g))
	{
		ready_to_eat++;
		if (ready_to_eat == g->n / 2 || turn == 2)
		{
			ready_to_eat = 0;
			turn++;
			turn %= 3;
			if (turn == 2 && !(g->n % 2))
				turn = 0;
		}
		monitor(p, g, EAT);
		milli_sleep(g->time_to_eat);
		pthread_mutex_unlock(g->forks + p->i);
		pthread_mutex_unlock(g->forks + (p->i + 1) % g->n);
		p->last_eat = get_time();
		p->n_eaten++;
		if (p->n_eaten == g->n_eat)
			g->ate_enough++;
	}
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
