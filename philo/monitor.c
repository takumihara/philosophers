#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#include "include/philo.h"
#include "include/utils.h"

static bool	is_starved(t_info *info, t_philo_info *ph_info);
static bool	all_satisfied(t_info *info);
static int	get_satisfied_philo(t_info *info);

void	monitor(t_info *info, t_philo_info *ph_info)
{
	while (true)
	{
		if (is_starved(info, ph_info))
			return ;
		if (all_satisfied(info))
			return ;
		usleep(500);
	}
}

bool	is_starved(t_info *info, t_philo_info *ph_info)
{
	int			i;
	bool		simulation_finished;
	long long	last_meal_time;

	i = 0;
	simulation_finished = false;
	while (i < info->num_of_philo && !simulation_finished)
	{
		last_meal_time = get_last_meal_time(&ph_info[i]);
		if (get_usec() - last_meal_time >= info->time_to_die)
		{
			pthread_mutex_lock(&info->mutex);
			info->simulation_finished = true;
			simulation_finished = true;
			pthread_mutex_unlock(&info->mutex);
		}
		if (simulation_finished)
			print_log(&ph_info[i], DIED);
		i++;
	}
	return (simulation_finished);
}

bool	all_satisfied(t_info *info)
{
	if (info->num_of_meal == 0
		|| get_satisfied_philo(info) == info->num_of_philo)
	{
		pthread_mutex_lock(&info->mutex);
		info->simulation_finished = true;
		pthread_mutex_unlock(&info->mutex);
		return (true);
	}
	return (false);
}

void	increment_satisfied_philo(t_info *info)
{
	pthread_mutex_lock(&info->mutex);
	info->satisfied_philo++;
	pthread_mutex_unlock(&info->mutex);
}

int	get_satisfied_philo(t_info *info)
{
	int	satisfied_philo;

	pthread_mutex_lock(&info->mutex);
	satisfied_philo = info->satisfied_philo;
	pthread_mutex_unlock(&info->mutex);
	return (satisfied_philo);
}
