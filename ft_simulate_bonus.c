/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_simulate_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kferterb <kferterb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 22:05:23 by kferterb          #+#    #+#             */
/*   Updated: 2022/03/03 16:28:52 by kferterb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	ft_philo_death(t_obj *o)
{
	int	i;

	i = -1;
	sem_wait(o->print);
	printf("%ld %d died\n", ft_get_time() - o->start_time,
		o->p[o->index].id);
	pthread_detach(o->death);
	pthread_detach(o->finish);
	sem_post(o->kill);
	while (++i <= o->count_philos)
		sem_post(o->end);
}

void	ft_philo_sleep_think(t_obj *o, int n)
{
	sem_wait(o->print);
	printf("%ld %d is sleeping\n", ft_get_time() - o->start_time,
		o->p[n].id);
	sem_post(o->print);
	my_usleep(o->time_to_sleep / 1000);
	sem_wait(o->print);
	printf("%ld %d is thinking\n", ft_get_time() - o->start_time,
		o->p[n].id);
	sem_post(o->print);
}

void	ft_philo_eat(t_obj *o, int n)
{
	sem_wait(o->fork);
	sem_wait(o->print);
	printf("%ld %d has taken a fork\n", ft_get_time() - o->start_time,
		o->p[n].id);
	sem_post(o->print);
	sem_wait(o->fork);
	sem_wait(o->print);
	printf("%ld %d has taken a fork\n", ft_get_time() - o->start_time,
		o->p[n].id);
	printf("%ld %d is eating\n", ft_get_time() - o->start_time,
		o->p[n].id);
	sem_post(o->print);
	o->p[n].time_last_lunch = ft_get_time();
	my_usleep(o->time_to_eat / 1000);
	sem_post(o->fork);
	sem_post(o->fork);
}

void	ft_start_live(t_obj *o)
{
	int	i;
	int	n;

	i = 0;
	n = o->index;
	pthread_create(&o->death, NULL, ft_check_dead, (void *)o);
	pthread_create(&o->finish, NULL, ft_finish_line, (void *)o);
	while (i < o->count_lunch || o->count_lunch == -1)
	{
		ft_philo_eat(o, n);
		i++;
		if (i == o->count_lunch)
		{
			pthread_detach(o->death);
			pthread_detach(o->finish);
			exit(0);
		}
		ft_philo_sleep_think(o, n);
	}
}
