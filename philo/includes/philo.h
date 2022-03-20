/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vrogiste <vrogiste@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/11 16:47:12 by vrogiste          #+#    #+#             */
/*   Updated: 2022/03/20 11:48:57 by vrogiste         ###   ########.fr       */
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
# include <stdint.h>
# include <limits.h>

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

typedef struct s_param
{
	int				number_of_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				number_of_eating;
	uint64_t		time_zero;
	bool			threads_ready;
	bool			death;
	pthread_mutex_t	mutex_ready;
	pthread_mutex_t	mutex_death;
	pthread_mutex_t	mutex_print;
}					t_param;

typedef struct s_philo
{
	int				index;
	pthread_mutex_t	mutex_fork;
	pthread_t		thread;
	t_param			*param;
	uint64_t		last_eat;
	pthread_mutex_t	mutex_last_eat;
}	t_philo;

/*circular_lst*/
t_list	*lst_new(void *content);
size_t	lst_size(t_list *lst);
void	lst_add_back(t_list **alst, t_list *new_node);

/*utils*/
int	atoi_error(char *str, bool *error);

#endif
