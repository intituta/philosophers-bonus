/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kferterb <kferterb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 21:52:37 by kferterb          #+#    #+#             */
/*   Updated: 2022/03/03 16:48:59 by kferterb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

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

void	ft_return(t_obj*o)
{
	int	i;

	i = 0;
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

void	ft_init_sem(char **argv, t_obj *o)
{
	sem_close(o->end);
	sem_close(o->kill);
	sem_close(o->fork);
	sem_close(o->print);
	sem_unlink("end");
	sem_unlink("kill");
	sem_unlink("forks");
	sem_unlink("print");
	o->end = sem_open("end", O_CREAT, 0644, 0);
	o->kill = sem_open("kill", O_CREAT, 0644, 0);
	o->print = sem_open("print", O_CREAT, 0644, 1);
	o->fork = sem_open("forks", O_CREAT, 0644, ft_atoi(argv[1]));
}

int	ft_init_philos(t_obj *o, int ac, char **av)
{
	int	i;

	i = -1;
	o->index = 0;
	o->count_lunch = -1;
	o->start_time = ft_get_time();
	o->count_philos = ft_atoi(av[1]);
	o->time_to_die = ft_atoi(av[2]) * 1000;
	o->time_to_eat = ft_atoi(av[3]) * 1000;
	o->time_to_sleep = ft_atoi(av[4]) * 1000;
	o->p = malloc(sizeof(t_philo) * o->count_philos);
	o->pid = malloc(sizeof(int *) * o->count_philos);
	if (!o->p || !o->pid)
		return (1);
	while (++i < o->count_philos)
		o->p[i].id = i + 1;
	if (ac == 5)
		return (0);
	o->count_lunch = ft_atoi(av[5]);
	return (0);
}

int	main(int ac, char **av)
{
	int			i;
	t_obj		o;
	pthread_t	free;

	i = -1;
	if ((ac != 5 && ac != 6) || ft_atoi(av[1]) <= 0
		|| ft_atoi(av[2]) < 60 || ft_atoi(av[3]) < 60
		|| ft_atoi(av[4]) < 60 || (ac == 6 && ft_atoi(av[5]) <= 0))
		return (write(1, "Error\n", 6), 1);
	if (ft_init_philos(&o, ac, av))
		return (1);
	ft_init_sem(av, &o);
	o.index = -1;
	o.start_time = ft_get_time();
	while (++o.index < o.count_philos)
	{
		o.p[o.index].time_last_lunch = o.start_time;
		o.pid[o.index] = fork();
		if (o.pid[o.index] == 0)
			ft_start_live(&o);
	}
	pthread_create(&free, NULL, free_all, (void *)&o);
	while (++i < o.count_philos)
		waitpid(o.pid[i], NULL, 0);
	ft_return(&o);
}
