#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <printf.h>

#include "include/philo.h"
#include "include/utils.h"

bool	init_program(int argc, char **argv, t_info *info, t_philo_info *ph_info)
{
	int	i;

	info->num_of_meal = -1;
	if (!((argc == 5 || argc == 6) &&
		  atoi_strict(argv[1], &info->num_of_philo) &&
		  info->num_of_philo >= 1 &&
		  info->num_of_philo <= MAX_NUM_OF_PHILO &&
		  atoi_strict(argv[2], &info->time_to_die) &&
		  atoi_strict(argv[3], &info->time_to_eat) &&
		  atoi_strict(argv[4], &info->time_to_sleep) &&
		  (argc == 5 || atoi_strict(argv[5], &info->num_of_meal))))
	{
		printf("argument error \n");
		return (false);
	}
	i = -1;
	while (++i < info->num_of_philo)
		init_philo_info(&ph_info[i], info, i + 1);
	info->simulation_finished = false;
	info->satisfied_philo = 0;
	if (!init_mutexes(&info->mutex, 1) ||
		!init_mutexes(info->forks, info->num_of_philo))
		return (false);
	return (true);
}

bool	init_philos(t_info *info, t_philo_info *ph_info, pthread_t *philos)
{
	int	i;

	i = -1;
	info->start = get_time();
	while (++i < info->num_of_philo)
	{
		ph_info[i].last_meal_time = info->start;
		if (pthread_create(&philos[i], NULL, philosopher, (void *)&ph_info[i]) != 0)
		{
			perror("philo");
			destroy_program(info, philos, i - 1);
			return (false);
		}
	}
	return (true);
}

bool	init_mutexes(pthread_mutex_t *mutexes, int mutex_num)
{
	int	i;

	i = -1;
	while (++i < mutex_num)
	{
		if (pthread_mutex_init(&mutexes[i], NULL) != 0)
		{
			perror("philo");
			destroy_mutexes(mutexes, i - 1);
			return (false);
		}
	}
	return (true);
}

void	init_philo_info(t_philo_info *ph_info, t_info *info, int id)
{
	ph_info->id = id;
	ph_info->left = id - 1;
	ph_info->right = id % info->num_of_philo;
	ph_info->last_meal_time = 0;
	ph_info->left_meal_cnt = info->num_of_meal;
	ph_info->common = info;
}
