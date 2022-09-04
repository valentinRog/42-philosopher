/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/11 16:47:12 by vrogiste          #+#    #+#             */
/*   Updated: 2022/09/04 20:55:25 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdatomic.h>
# include <string.h>

# define PHILO_MAX 1000
# define ARG_MAX 2

# define FORK "has taken a fork"
# define EAT "is eating"
# define SLEEP "is sleeping"
# define THINK "is thinking"

typedef struct s_arg
{
	void	*content[ARG_MAX];
}	t_arg;

typedef struct s_philo
{
	int			i;
	atomic_int	last_eat;
	int			n_eaten;
}	t_philo;

typedef struct s_game
{
	int				n;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				n_eat;
	int				t0;
	atomic_int		ate_enough;
	atomic_bool		running;
	t_philo			philos[PHILO_MAX];
	pthread_mutex_t	forks[PHILO_MAX];
	pthread_t		threads[PHILO_MAX];
	pthread_mutex_t	print_lock;
	pthread_mutex_t	start_lock;
}	t_game;

/* routine */
void	*philoop(void *arg);

/* utils */
int		ft_atoi(const char *str);
int		get_time(void);
void	milli_sleep(int time);

#endif
