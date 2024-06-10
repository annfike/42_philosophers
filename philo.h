/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adelaloy <adelaloy <adelaloy@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 13:30:18 by adelaloy          #+#    #+#             */
/*   Updated: 2024/05/25 18:11:16 by adelaloy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_philo
{
	int				id;
	int				had_meals;
	pthread_mutex_t	*left;
	pthread_mutex_t	*right;
	struct s_data	*data;
	long			start_time;
	long			last_eat;
}					t_philo;

typedef struct s_data
{
	int				number_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				meals_must_eat;
	pthread_mutex_t	*forks;
	pthread_mutex_t	printf_mutex;
	pthread_mutex_t	meal_mutex;
	pthread_mutex_t	dead_mutex;
	pthread_mutex_t	total_mutex;
	t_philo			*philos;
	int				dead;
	int				total_meals;
	long			start_time;
}					t_data;

/* utils.c */
long				ft_atoi(const char *str);
int					ft_is_digit(char *str);
int					ft_usleep(long time);

/* init.c */
int					check_args(int argc, char **argv);
int					parse_argv(int argc, char **argv, t_data *data);
void				init_mutex(t_data *data);
long				get_time(void);
void				init_philos(t_data *data);

/* process.c */
int					get_dead(t_data *data);
void				print_process(t_philo *philo, char *str);
void				eating(t_philo *philo);
void				*philo_process(void *arg);

#endif