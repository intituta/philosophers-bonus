/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_simulate_bonus2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kferterb <kferterb@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 12:33:35 by kferterb          #+#    #+#             */
/*   Updated: 2022/03/02 22:14:08 by kferterb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*free_all(void *data)
{
	int		i;
	t_obj	*o;

	o = (t_obj *)data;
	sem_wait(o->kill);
	i = -1;
	while (++i < o->count_philos)
		kill(o->pid[i], 9);
	free(o->p);
	free(o->pid);
	sem_close(o->fork);
	sem_unlink("forks");
	sem_close(o->end);
	sem_unlink("end");
	sem_close(o->print);
	sem_unlink("print");
	exit(0);
}

long	ft_get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((long)tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	my_usleep(long time)
{
	long	t;

	t = ft_get_time();
	while (ft_get_time() - t < time)
		usleep(100);
}

void	*ft_finish_line(void *data)
{
	t_obj	*o;

	o = (t_obj *)data;
	sem_wait(o->end);
	exit (0);
}

void	*ft_check_dead(void *data)
{
	t_obj	*o;
	int		time;
	int		done;

	o = (t_obj *)data;
	done = 0;
	while (done == 0)
	{
		time = (ft_get_time()
				- o->p[o->index].time_last_lunch) * 1000;
		if (time > o->time_to_die)
			ft_philo_death(o);
	}
	return (0);
}
