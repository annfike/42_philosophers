/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adelaloy <adelaloy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 17:01:35 by adelaloy          #+#    #+#             */
/*   Updated: 2024/05/27 10:45:40 by adelaloy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	stop_all_full(t_data *data)
{
	pthread_mutex_lock(&data->dead_mutex);
	data->dead = 1;
	pthread_mutex_unlock(&data->dead_mutex);
	pthread_mutex_unlock(&data->total_mutex);
}

void	stop_dead(t_data *data, int i)
{
	pthread_mutex_lock(&data->printf_mutex);
	printf("%ld %d died\n", get_time() - data->philos[i].start_time,
		data->philos[i].id + 1);
	pthread_mutex_unlock(&data->printf_mutex);
	pthread_mutex_lock(&data->dead_mutex);
	data->dead = 1;
	pthread_mutex_unlock(&data->dead_mutex);
	pthread_mutex_unlock(&data->meal_mutex);
}

void	monitor(t_data *data)
{
	int	i;

	while (1)
	{
		i = 0;
		while (i < data->number_philos)
		{
			pthread_mutex_lock(&data->total_mutex);
			if (data->total_meals == data->number_philos)
			{
				stop_all_full(data);
				return ;
			}
			pthread_mutex_unlock(&data->total_mutex);
			pthread_mutex_lock(&data->meal_mutex);
			if (get_time() - data->philos[i].last_eat > data->time_to_die)
			{
				stop_dead(data, i);
				return ;
			}
			pthread_mutex_unlock(&data->meal_mutex);
			i++;
		}
	}
}

void	destroy_mutex(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_philos)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&data->printf_mutex);
	pthread_mutex_destroy(&data->meal_mutex);
	pthread_mutex_destroy(&data->dead_mutex);
	pthread_mutex_destroy(&data->total_mutex);
}

int	main(int argc, char **argv)
{
	t_data		data;
	pthread_t	*thread;
	int			i;

	i = -1;
	if (parse_argv(argc, argv, &data))
		return (1);
	init_mutex(&data);
	init_philos(&data);
	thread = malloc(sizeof(pthread_t) * data.number_philos);
	while (++i < data.number_philos)
		pthread_create(&thread[i], NULL, philo_process, &data.philos[i]);
	monitor(&data);
	i = -1;
	while (++i < data.number_philos)
		pthread_join(thread[i], NULL);
	destroy_mutex(&data);
	free(data.forks);
	free(data.philos);
	free(thread);
	return (0);
}

/*
number_of_philosophers time_to_die time_to_eat time_to_sleep
[number_of_times_each_philosopher_must_eat]

gcc -Wall -Wextra -Werror -pthread main.c
gcc -Wall -Wextra -Werror -fsanitize=thread main.c
leaks --atExit -- ./philo 21 290 100 100
valgrind --leak-check=full ./philo 5 290 100 100
valgrind --tool=drd ./philo 30 890 100 100 3
valgrind --tool=helgrind ./philo 30 890 100 100 3
./a.out 5 202 100 100


тест (поменять philo -> philo_one)
https://github.com/cacharle/philosophers_test
https://github.com/kichkiro/philosophers_tester

визуализатор
https://nafuka11.github.io/philosophers-visualizer/

варианты:
1.четные философы берут сначала правую вилку, 
нечетные - левую (кладут в обратном порядке) 
2.философы берут вилку с минимальным айди, кладут обратно с максимальным
3.четные приходят к столу с опозданием ( usleep time_to_eat/2)
4.все берут левую, кроме последнего
5.четные начинают цикл со сна (под вопросом - можно ли так делать)

struct timeval
    long   tv_sec; //время, прошедшее с 1970 года в секундах (1,6 млрд...)
    long   tv_usec; //время, прошедшее с 1970 года в микросекундах 
    (между отдельными секундами, 0-999999)
    
    1 миллисекунда = 1000 микросекунд. 1 микросекунда = 0.001 миллисекунд. 
    1 секунда = 1000000 микросекунд = 1000 миллисекунд
    tv_sec * 1000 + tv_usec / 1000 = миллисекунды
    usleep в микросекундах
    в начале проги определить текущее время и потом от него отталкиваться 
    int gettimeofday(struct timeval *tv, struct timezone *tz);
*/

/*
не должны умереть
./philo 5 800 200 200 7
./philo 4 410 200 200 7
должны
./philo 1 800 200 200 7
./philo 4 310 200 100 7

No philosopher should die.

./philo 5 900 295 60 10
    ARGS: 19 210 69 139 10
*/