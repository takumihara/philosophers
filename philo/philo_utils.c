#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>

#include "include/philo.h"
#include "include/utils.h"

bool	is_simulation_finished(const t_philo_info *ph_info)
{
	bool	simulation_finished;

	simulation_finished = false;
	pthread_mutex_lock(&ph_info->common->mutex);
	if (ph_info->common->simulation_finished)
		simulation_finished = true;
	pthread_mutex_unlock(&ph_info->common->mutex);
	return (simulation_finished);
}

void	print_log(const t_philo_info *info, t_philo_status status)
{
	const long long	timestamp = get_mtime() - info->common->start;

	pthread_mutex_lock(&info->common->mutex);
	if (status != DIED && info->common->simulation_finished)
	{
		pthread_mutex_unlock(&info->common->mutex);
		return ;
	}
	if (status == TAKEN_FORK)
		printf("%lld %d has taken a fork\n", timestamp, info->id);
	else if (status == EATING)
		printf("%lld %d is eating\n", timestamp, info->id);
	else if (status == SLEEPING)
		printf("%lld %d is sleeping\n", timestamp, info->id);
	else if (status == THINKING)
		printf("%lld %d is thinking\n", timestamp, info->id);
	else if (status == DIED)
		printf("%lld %d died\n", timestamp, info->id);
	pthread_mutex_unlock(&info->common->mutex);
}

int	calc_interval(const t_philo_info *ph_info)
{
	const int	num_of_philo = ph_info->common->num_of_philo;
	const int	time_to_eat = ph_info->common->time_to_eat;
	const int	time_to_sleep = ph_info->common->time_to_sleep;
	const int	n = num_of_philo / 2;
	const int	time_to_think = time_to_eat * num_of_philo
					- (time_to_eat + time_to_sleep) * n;

	return (time_to_think / n);
}

void	set_last_meal_time(t_philo_info *ph_info, long long last_meal_time)
{
	pthread_mutex_lock(&ph_info->common->mutex);
	ph_info->last_meal_time = last_meal_time;
	pthread_mutex_unlock(&ph_info->common->mutex);
}

long long	get_last_meal_time(t_philo_info *ph_info)
{
	long long	last_meal_time;

	pthread_mutex_lock(&ph_info->common->mutex);
	last_meal_time = ph_info->last_meal_time;
	pthread_mutex_unlock(&ph_info->common->mutex);
	return (last_meal_time);
}
