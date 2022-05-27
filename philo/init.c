#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <printf.h>

#include "include/philo.h"

pthread_t	*init_philo(int num_of_philo)
{
	pthread_t	*philos;

	philos = malloc(sizeof(pthread_t) * num_of_philo);
	return (philos);
}

pthread_mutex_t	*init_fork(int num_of_philo)
{
	pthread_mutex_t *forks;
	int				i;

	forks = malloc(sizeof(pthread_mutex_t) * num_of_philo);
	if (!forks)
		return (NULL);
	i = -1;
	while (++i < num_of_philo)
		pthread_mutex_init(&forks[i], NULL);
	return (forks);
}

void	init_philo_info(t_philo_info *ph_info, t_info *info, int id, long start)
{
	ph_info->common = info;
	ph_info->id = id;
	ph_info->left = id - 1;
	ph_info->right = id % info->num_of_philo;
	ph_info->last_meal_time = start;
	ph_info->left_meal_cnt = info->num_of_meal;
}
