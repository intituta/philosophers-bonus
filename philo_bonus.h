/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kferterb <kferterb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 21:55:08 by kferterb          #+#    #+#             */
/*   Updated: 2022/03/03 13:36:29 by kferterb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <pthread.h>
# include <sys/time.h>
# include <semaphore.h>

typedef struct s_philo
{
	int			id;
	long		time_last_lunch;
}	t_philo;

typedef struct s_obj
{
	int			*pid;
	int			index;
	int			time_to_die;
	int			time_to_eat;
	int			count_lunch;
	int			count_philos;
	int			time_to_sleep;
	long		start_time;
	sem_t		*end;
	sem_t		*kill;
	sem_t		*fork;
	sem_t		*print;
	pthread_t	death;
	pthread_t	finish;
	t_philo		*p;
}	t_obj;

long	ft_get_time(void);

int		main(int ac, char **av);
int		ft_atoi(const char *str);
int		ft_init_philos(t_obj *o, int ac, char **av);

void	ft_return(t_obj*o);
void	my_usleep(long time);
void	*free_all(void *data);
void	ft_start_live(t_obj *o);
void	ft_philo_death(t_obj *o);
void	*ft_check_dead(void *data);
void	ft_create_process(t_obj *o);
void	*ft_finish_line(void *data);
void	ft_philo_eat(t_obj *o, int n);
void	ft_philo_sleep_think(t_obj *o, int n);
void	ft_init_sem(char **argv, t_obj *o);

#endif