/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adelaloy <adelaloy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 15:05:41 by adelaloy          #+#    #+#             */
/*   Updated: 2024/05/27 11:45:01 by adelaloy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_args(int argc, char **argv)
{
	int	i;

	i = 1;
	while (argv[i])
	{
		if (!ft_is_digit(argv[i]))
		{
			write(2, "Error\n", 6);
			return (1);
		}
		i++;
	}
	if (argc != 5 && argc != 6)
	{
		write(2, "Error\n", 6);
		return (1);
	}
	return (0);
}

int	parse_argv(int argc, char **argv, t_data *data)
{
	if (check_args(argc, argv))
		return (1);
	data->number_philos = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	data->meals_must_eat = 0;
	if (argc == 6)
		data->meals_must_eat = ft_atoi(argv[5]);
	if (data->number_philos < 1 || data->time_to_die < 0
		|| data->time_to_eat < 0 || data->time_to_sleep < 0 || (argc == 6
			&& data->meals_must_eat < 1))
	{
		write(2, "Error\n", 6);
		return (1);
	}
	data->dead = 0;
	data->total_meals = 0;
	if (data->number_philos == 1)
		data->total_meals = -1;
	data->start_time = get_time();
	return (0);
}

void	init_mutex(t_data *data)
{
	int				i;
	pthread_mutex_t	*mutex;

	mutex = malloc(sizeof(pthread_mutex_t) * data->number_philos);
	i = 0;
	while (i < data->number_philos)
	{
		pthread_mutex_init(&mutex[i], NULL);
		i++;
	}
	data->forks = mutex;
	pthread_mutex_init(&data->printf_mutex, NULL);
	pthread_mutex_init(&data->meal_mutex, NULL);
	pthread_mutex_init(&data->dead_mutex, NULL);
	pthread_mutex_init(&data->total_mutex, NULL);
}

long	get_time(void)
{
	struct timeval	current_time;
	long			time_ms;

	gettimeofday(&current_time, NULL);
	time_ms = current_time.tv_sec * 1000 + current_time.tv_usec / 1000;
	return (time_ms);
}

void	init_philos(t_data *data)
{
	int		i;
	t_philo	*philos;

	philos = malloc(sizeof(t_philo) * data->number_philos);
	i = 0;
	while (i < (data->number_philos))
	{
		philos[i].id = i;
		philos[i].left = &data->forks[philos[i].id];
		philos[i].right = &data->forks[(philos[i].id + 1)
			% data->number_philos];
		philos[i].had_meals = 0;
		philos[i].data = data;
		philos[i].last_eat = data->start_time;
		philos[i].start_time = data->start_time;
		i++;
	}
	data->philos = philos;
}
