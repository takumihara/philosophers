#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <printf.h>

#include "include/philo.h"
#include "include/utils.h"

static bool	ph_grab_odd_forks(const t_philo_info *ph_info);
static bool	ph_grab_even_forks(const t_philo_info *ph_info);
static bool	ph_grab_first_odd_forks(const t_philo_info *ph_info);
static bool	ph_grab_first_even_forks(const t_philo_info *ph_info);

bool	ph_grab_forks(const t_philo_info *ph_info, bool *first_forks)
{
	if (*first_forks)
	{
		*first_forks = false;
		if (ph_info->common->num_of_philo % 2)
			return (ph_grab_first_odd_forks(ph_info));
		else
			return (ph_grab_first_even_forks(ph_info));
	}
	else
	{
		if (ph_info->common->num_of_philo % 2)
			return (ph_grab_odd_forks(ph_info));
		else
			return (ph_grab_even_forks(ph_info));
	}
}

bool	ph_grab_odd_forks(const t_philo_info *ph_info)
{
	pthread_mutex_t	*forks;
	long			interval;

	if (ph_info->left == ph_info->right)
		return (false);
	forks = ph_info->common->forks;
	interval = max(0, calc_interval(ph_info));
	sleep_precisely(get_time(), interval);
	pthread_mutex_lock(&forks[ph_info->left]);
	pthread_mutex_lock(&forks[ph_info->right]);
	return (true);
}

bool	ph_grab_even_forks(const t_philo_info *ph_info)
{
	pthread_mutex_t		*forks;
	const long			time_to_sleep = ph_info->common->time_to_sleep;

	forks = ph_info->common->forks;
	if (time_to_sleep == 0)
		usleep(500);
	if (ph_info->id % 2)
		pthread_mutex_lock(&forks[ph_info->left]);
	else
	{
		usleep(200);
		pthread_mutex_lock(&forks[ph_info->right]);
	}
	print_log(ph_info, TAKEN_FORK);
	if (ph_info->id % 2)
	{
		usleep(200);
		pthread_mutex_lock(&forks[ph_info->right]);
	}
	else
		pthread_mutex_lock(&forks[ph_info->left]);
	print_log(ph_info, TAKEN_FORK);
	return (true);
}

bool	ph_grab_first_odd_forks(const t_philo_info *ph_info)
{
	const t_info	*info = ph_info->common;
	pthread_mutex_t	*forks;
	long			interval;
	long			one_loop;
	long			next;

	if (ph_info->left == ph_info->right)
		return (false);
	forks = ph_info->common->forks;
	interval = max(0, calc_interval(ph_info));
	one_loop = interval + info->time_to_eat + info->time_to_sleep;
	next = (ph_info->id - 1) * info->time_to_eat % one_loop;
	sleep_precisely(info->start, next);
	pthread_mutex_lock(&forks[ph_info->left]);
	print_log(ph_info, TAKEN_FORK);
	pthread_mutex_lock(&forks[ph_info->right]);
	print_log(ph_info, TAKEN_FORK);
	return (true);
}

bool	ph_grab_first_even_forks(const t_philo_info *ph_info)
{
	pthread_mutex_t		*forks;

	forks = ph_info->common->forks;
	if (ph_info->id % 2 == 0)
		return (false);
	pthread_mutex_lock(&forks[ph_info->left]);
	print_log(ph_info, TAKEN_FORK);
	pthread_mutex_lock(&forks[ph_info->right]);
	print_log(ph_info, TAKEN_FORK);
	return (true);
}
