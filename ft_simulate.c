/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_simulate.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kferterb <kferterb@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/03 19:46:45 by kferterb          #+#    #+#             */
/*   Updated: 2022/03/02 18:32:26 by kferterb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	ft_get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((long)tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	*ft_check_philos(void *data)
{
	int		i;
	t_obj	*o;

	o = (t_obj *)data;
	while (1)
	{
		i = -1;
		while (++i < o->count)
		{
			if (ft_get_time() - o->p[i].time_last_lunch > o->p[i].time_to_die)
			{
				o->check_death = 1;
				if (o->flag == 0
					|| (o->flag == 1 && o->p[i].count_of_lunch > 0))
					printf("%ld %d died\n",
						ft_get_time() - o->start_time, o->p[i].id);
				return ((void *)0);
			}
		}
	}
}

void	ft_eat_sleep_think(t_philo *p, t_obj *o, int i)
{
	pthread_mutex_lock(&o->lock);
	p->time_last_lunch = ft_get_time();
	if (o->check_death == 0)
		printf("%ld %d is eating\n", ft_get_time() - o->start_time, p->id);
	pthread_mutex_unlock(&o->lock);
	ft_usleep(p->time_to_eat);
	if (p->id == 1)
		pthread_mutex_unlock(&o->p[o->count - 1].mutex);
	else
		pthread_mutex_unlock(&o->p[i - 1].mutex);
	pthread_mutex_unlock(&o->p[i].mutex);
	pthread_mutex_lock(&o->lock);
	if ((o->flag == 0 && o->check_death == 0)
		|| (o->check_death == 0 && o->flag == 1 && p->count_of_lunch != 1))
		printf("%ld %d is sleeping\n", ft_get_time() - o->start_time, p->id);
	pthread_mutex_unlock(&o->lock);
	ft_usleep(p->time_to_sleep);
	pthread_mutex_lock(&o->lock);
	if ((o->flag == 0 && o->check_death == 0)
		|| (o->check_death == 0 && o->flag == 1 && p->count_of_lunch != 1))
		printf("%ld %d is thinking\n", ft_get_time() - o->start_time, p->id);
	pthread_mutex_unlock(&o->lock);
	p->count_of_lunch--;
}

void	*ft_start_live(void *data)
{
	int		i;
	t_obj	*o;

	o = (t_obj *)data;
	i = o->index;
	o->p[i].count_of_lunch = o->count_lunch;
	o->p[i].time_last_lunch = ft_get_time();
	while (o->check_death != 1)
	{
		if (o->flag == 1 && o->p[i].count_of_lunch == 0)
			return ((void *)0);
		if (o->p[i].id == 1)
			pthread_mutex_lock(&o->p[o->count - 1].mutex);
		else
			pthread_mutex_lock(&o->p[i - 1].mutex);
		if (o->check_death == 0)
			printf("%ld %d has taken a fork\n",
				ft_get_time() - o->start_time, o->p[i].id);
		pthread_mutex_lock(&o->p[i].mutex);
		if (o->check_death == 0)
			printf("%ld %d has taken a fork\n",
				ft_get_time() - o->start_time, o->p[i].id);
		ft_eat_sleep_think(&o->p[i], o, i);
	}
	return ((void *)0);
}

int	ft_start_simulate(pthread_t *thread, t_obj *o)
{
	o->index = -1;
	o->start_time = ft_get_time();
	while (++o->index < o->count)
		o->p[o->index].time_last_lunch = o->start_time;
	o->index = 0;
	while (o->index < o->count)
	{
		if (pthread_create(&thread[o->index], NULL, ft_start_live, o) != 0)
			return (1);
		usleep(50);
		o->index += 2;
	}
	o->index = 1;
	while (o->index < o->count)
	{
		if (pthread_create(&thread[o->index], NULL, ft_start_live, o) != 0)
			return (1);
		usleep(50);
		o->index += 2;
	}
	return (0);
}
