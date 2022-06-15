#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

#include "include/philo.h"
#include "include/utils.h"
#include "include/unwrap.h"

static bool	ph_grab_odd_forks(const t_philo_info *ph_info);
static bool	ph_grab_even_forks(const t_philo_info *ph_info);
static bool	ph_grab_first_odd_forks(const t_philo_info *ph_info);
static bool	ph_grab_first_even_forks(const t_philo_info *ph_info);

bool	ph_grab_forks(const t_philo_info *ph_info, bool *is_first_fork)
{
	if (*is_first_fork)
	{
		*is_first_fork = false;
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

	if (ph_info->left == ph_info->right)
		return (false);
	forks = ph_info->common->forks;
	usleep_until(get_usec() + ph_info->common->time_to_think);
	if (ph_info->id % 2)
		pthread_mutex_lock_unwrap(&forks[ph_info->left]);
	else
		pthread_mutex_lock_unwrap(&forks[ph_info->right]);
	print_log(ph_info, TAKEN_FORK);
	if (ph_info->id % 2)
		pthread_mutex_lock_unwrap(&forks[ph_info->right]);
	else
		pthread_mutex_lock_unwrap(&forks[ph_info->left]);
	print_log(ph_info, TAKEN_FORK);
	return (true);
}

bool	ph_grab_even_forks(const t_philo_info *ph_info)
{
	pthread_mutex_t		*forks;

	forks = ph_info->common->forks;
	if (ph_info->id % 2)
		pthread_mutex_lock_unwrap(&forks[ph_info->left]);
	else
	{
		usleep(200);
		pthread_mutex_lock_unwrap(&forks[ph_info->right]);
	}
	print_log(ph_info, TAKEN_FORK);
	if (ph_info->id % 2)
	{
		usleep(200);
		pthread_mutex_lock_unwrap(&forks[ph_info->right]);
	}
	else
		pthread_mutex_lock_unwrap(&forks[ph_info->left]);
	print_log(ph_info, TAKEN_FORK);
	return (true);
}

bool	ph_grab_first_odd_forks(const t_philo_info *ph_info)
{
	const t_info	*info = ph_info->common;
	pthread_mutex_t	*forks;
	long long		one_loop;
	long long		next_eat_time;

	if (ph_info->left == ph_info->right)
		return (false);
	forks = ph_info->common->forks;
	one_loop = info->time_to_think + info->time_to_eat + info->time_to_sleep;
	next_eat_time = (ph_info->id - 1) * info->time_to_eat % one_loop;
	usleep_until(info->start + next_eat_time);
	if (ph_info->id % 2)
		pthread_mutex_lock_unwrap(&forks[ph_info->left]);
	else
		pthread_mutex_lock_unwrap(&forks[ph_info->right]);
	print_log(ph_info, TAKEN_FORK);
	if (ph_info->id % 2)
		pthread_mutex_lock_unwrap(&forks[ph_info->right]);
	else
		pthread_mutex_lock_unwrap(&forks[ph_info->left]);
	print_log(ph_info, TAKEN_FORK);
	return (true);
}

bool	ph_grab_first_even_forks(const t_philo_info *ph_info)
{
	pthread_mutex_t		*forks;

	forks = ph_info->common->forks;
	if (ph_info->id % 2 == 0)
	{
		usleep_until(get_usec() + ph_info->common->time_to_eat / 2);
		return (false);
	}
	pthread_mutex_lock_unwrap(&forks[ph_info->left]);
	print_log(ph_info, TAKEN_FORK);
	pthread_mutex_lock_unwrap(&forks[ph_info->right]);
	print_log(ph_info, TAKEN_FORK);
	return (true);
}
