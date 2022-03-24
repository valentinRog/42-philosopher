/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vrogiste <vrogiste@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/11 16:47:12 by vrogiste          #+#    #+#             */
/*   Updated: 2022/03/24 11:08:01 by vrogiste         ###   ########.fr       */
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
# include <limits.h>
# include <inttypes.h>
# include <semaphore.h>

# define PHILO_MAX 50

enum
{
	FORK,
	EAT,
	SLEEP,
	THINK,
	DIE,
};

typedef struct s_list
{
	void			*content;
	struct s_list	*prev;
	struct s_list	*next;
}	t_list;

# define SEM_READY "sem_ready"
# define SEM_FORKS "sem_forks"
# define SEM_PRINT "sem_print"
# define SEM_LAST_EAT "sem_last_eat"

typedef struct s_param
{
	int			number_of_philo;
	int			time_to_die;
	int			time_to_eat;
	int			time_to_sleep;
	int			number_of_eating;
	uint64_t	time_zero;
	sem_t		*sem_ready;
	sem_t		*sem_forks;
	sem_t		*sem_print;
	sem_t		*sem_last_eat;
}				t_param;

/*utils*/
bool		atoi_error(char *str, int *nb);
uint64_t	get_time(void);
void		micro_sleep(uint64_t time);

/*init*/
bool		fill_param(t_param *param, int argc, char **argv);
bool		init_process(t_param *param);

/*process*/
void	process(t_param *param, int index);

#endif
