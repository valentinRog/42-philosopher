/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vrogiste <vrogiste@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/11 16:47:12 by vrogiste          #+#    #+#             */
/*   Updated: 2022/03/25 16:41:34 by vrogiste         ###   ########.fr       */
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
# include <signal.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <string.h>

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
	int			n_philo;
	int			time_to_die;
	int			time_to_eat;
	int			time_to_sleep;
	int			number_of_eating;
	uint64_t	time_zero;
	uint64_t	last_eat;
	sem_t		*sem_ready;
	sem_t		*sem_forks;
	sem_t		*sem_print;
	sem_t		*sem_last_eat;
}				t_param;

typedef struct s_philo
{
	int		index;
	int		pid;
	int		n_eaten;
	sem_t	*sem_n_eaten;
	char	*sem_n_eaten_name;
	t_param	*param;
}	t_philo;

/*utils*/
bool		atoi_error(char *str, int *nb);
uint64_t	get_time(void);
void		micro_sleep(uint64_t time);

/*itoa*/
char		*ft_itoa(int n);

/*init*/
bool		init_param(t_param *param, int argc, char **argv);
bool		init_philo(t_param *param, t_list **alst);
bool		init_process(t_list *lst);

/*process*/
void		process(t_philo *philo);

/*circular_lst*/
t_list		*lst_new(void *content);
size_t		lst_size(t_list *lst);
void		lst_add_back(t_list **alst, t_list *new_node);

/*clear*/
void		unlink_param(void);
void		lst_clear(t_list *lst);

#endif
