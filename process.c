/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adelaloy <adelaloy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 15:08:10 by adelaloy          #+#    #+#             */
/*   Updated: 2024/05/27 10:35:08 by adelaloy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	get_dead(t_data *data)
{
	int	dead;

	pthread_mutex_lock(&data->dead_mutex);
	dead = data->dead;
	pthread_mutex_unlock(&data->dead_mutex);
	return (dead);
}

void	print_process(t_philo *philo, char *str)
{
	pthread_mutex_lock(&philo->data->printf_mutex);
	pthread_mutex_lock(&philo->data->total_mutex);
	if (get_dead(philo->data) == 1
		|| philo->data->total_meals >= philo->data->number_philos)
	{
		pthread_mutex_unlock(&philo->data->printf_mutex);
		pthread_mutex_unlock(&philo->data->total_mutex);
		return ;
	}
	printf("%ld %d %s\n", get_time() - philo->start_time, philo->id + 1, str);
	pthread_mutex_unlock(&philo->data->total_mutex);
	pthread_mutex_unlock(&philo->data->printf_mutex);
}

void	eating(t_philo *philo)
{
	pthread_mutex_lock(philo->right);
	print_process(philo, "has taken a fork");
	if (philo->data->number_philos == 1)
	{
		pthread_mutex_unlock(philo->right);
		pthread_mutex_lock(&philo->data->dead_mutex);
		philo->data->dead = 1;
		pthread_mutex_unlock(&philo->data->dead_mutex);
		return ;
	}
	pthread_mutex_lock(philo->left);
	print_process(philo, "has taken a fork");
	pthread_mutex_lock(&philo->data->meal_mutex);
	philo->last_eat = get_time();
	pthread_mutex_unlock(&philo->data->meal_mutex);
	print_process(philo, "is eating");
	ft_usleep(philo->data->time_to_eat);
	philo->had_meals++;
	pthread_mutex_unlock(philo->right);
	pthread_mutex_unlock(philo->left);
}

void	*philo_process(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 != 0)
		ft_usleep(philo->data->time_to_eat / 1.05);
	while (!get_dead(philo->data))
	{
		eating(philo);
		if (philo->had_meals == philo->data->meals_must_eat)
		{
			pthread_mutex_lock(&philo->data->printf_mutex);
			pthread_mutex_lock(&philo->data->total_mutex);
			philo->data->total_meals++;
			pthread_mutex_unlock(&philo->data->total_mutex);
			pthread_mutex_unlock(&philo->data->printf_mutex);
		}
		print_process(philo, "is sleeping");
		ft_usleep(philo->data->time_to_sleep);
		print_process(philo, "is thinking");
	}
	return (NULL);
}
