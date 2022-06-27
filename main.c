/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kferterb <kferterb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 17:59:21 by kferterb          #+#    #+#             */
/*   Updated: 2022/03/03 16:49:51 by kferterb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *str)
{
	int			i;
	int			k;
	long int	j;

	i = 0;
	j = 0;
	k = 1;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			k = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		j = j * 10 + k * (str[i] - '0');
		i++;
		if (j < -2147483648 || j > 2147483647)
			exit(write(1, "Error\n", 6));
	}
	return (j);
}

void	ft_usleep(int time)
{
	long	end;
	long	start;

	start = ft_get_time();
	end = start + time;
	while (start < end)
	{
		usleep(500);
		start = ft_get_time();
	}
}

int	ft_free_all(t_obj *o, int flag, pthread_t *thread)
{
	int	i;

	i = -1;
	if (o->p)
	{
		while (++i < o->count - 1)
			pthread_mutex_destroy(&(o->p[i].mutex));
		free(o->p);
		if (thread)
			free(thread);
		if (flag == 0)
			return (0);
		write(1, "Error\n", 6);
	}
	return (1);
}

int	ft_init_philos(t_obj *o, int ac, char **av)
{
	int	i;

	i = -1;
	o->flag = 0;
	o->check_death = 0;
	o->count = ft_atoi(av[1]);
	o->p = malloc(sizeof(t_philo) * o->count);
	if (!o->p || pthread_mutex_init(&o->lock, NULL) != 0)
		return (1);
	while (++i < o->count)
	{
		if (pthread_mutex_init(&o->p[i].mutex, NULL) != 0)
			return (1);
		o->p[i].id = i + 1;
		o->p[i].time_to_die = ft_atoi(av[2]);
		o->p[i].time_to_eat = ft_atoi(av[3]);
		o->p[i].time_to_sleep = ft_atoi(av[4]);
	}
	if (ac == 5)
		return (0);
	o->flag = 1;
	o->count_lunch = ft_atoi(av[5]);
	return (0);
}

int	main(int ac, char **av)
{
	t_obj		o;
	pthread_t	check;
	pthread_t	*thread;

	if ((ac != 5 && ac != 6) || ft_atoi(av[1]) <= 0
		|| ft_atoi(av[2]) < 60 || ft_atoi(av[3]) < 60
		|| ft_atoi(av[4]) < 60 || (ac == 6 && ft_atoi(av[5]) <= 0))
		return (write(1, "Error\n", 6), 1);
	if (ft_init_philos(&o, ac, av))
		return (1);
	thread = malloc(sizeof(pthread_t *) * o.count);
	if (!thread || (pthread_create(&check, NULL, ft_check_philos, &o) != 0)
		|| ft_start_simulate(thread, &o) == 1)
		return (ft_free_all(&o, 1, 0));
	o.index = 0;
	while (o.index < o.count && o.count > 1)
		pthread_join(thread[o.index++], NULL);
	pthread_join(check, NULL);
	ft_free_all(&o, 0, thread);
	return (0);
}
