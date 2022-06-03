#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>

#include "include/philo.h"
#include "include/utils.h"

bool	is_starved(const t_philo_info *ph_info)
{
	bool	is_starved;

	sem_wait(ph_info->common->sem);
	is_starved = ph_info->is_starved;
	sem_post(ph_info->common->sem);
	return (is_starved);
}

void	print_log(const t_philo_info *ph_info, t_philo_status status)
{
	const long long	timestamp = get_mtime() - ph_info->common->start;

	 if (status != DIED && ph_info->simulation_finished)
		return ;
	sem_wait(ph_info->common->sem_out);
	if (status == TAKEN_FORK)
		printf("%lld %d has taken a fork\n", timestamp, ph_info->id);
	else if (status == EATING)
		printf("%lld %d is eating\n", timestamp, ph_info->id);
	else if (status == SLEEPING)
		printf("%lld %d is sleeping\n", timestamp, ph_info->id);
	else if (status == THINKING)
		printf("%lld %d is thinking\n", timestamp, ph_info->id);
	else if (status == DIED)
		printf("%lld %d died\n", timestamp, ph_info->id);
	if (status != DIED)
		sem_post(ph_info->common->sem_out);
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
	sem_wait(ph_info->common->sem);
	ph_info->last_meal_time = last_meal_time;
	sem_post(ph_info->common->sem);
}

long long	get_last_meal_time(t_philo_info *ph_info)
{
	long long	last_meal_time;

	sem_wait(ph_info->common->sem);
	last_meal_time = ph_info->last_meal_time;
	sem_post(ph_info->common->sem);
	return (last_meal_time);
}
