#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <printf.h>

#include "include/philo.h"
#include "include/utils.h"

bool	ph_grab_forks(const t_philo_info *ph_info);
void	ph_drop_forks(const t_philo_info *ph_info);
void	ph_eat(t_philo_info *ph_info);
void	ph_sleep(const t_philo_info *ph_info);
void	ph_think(const t_philo_info *ph_info);
void	ph_wait(long wait_time);
bool	is_simulation_finished(const t_philo_info *ph_info);

void *philosopher(void *arg_)
{
	t_philo_info 	*ph_info = (t_philo_info*)arg_;

	while (1)
	{
		if (is_simulation_finished(ph_info))
			return (NULL);
		if (!ph_grab_forks(ph_info))
			continue;
		ph_eat(ph_info);
		ph_drop_forks(ph_info);
		ph_sleep(ph_info);
		ph_think(ph_info);
	}
}

// grab_forks() return false when the left and right forks are the same
// this is a part of algorithm to prevent a deadlock
bool ph_grab_forks(const t_philo_info *ph_info)
{
	pthread_mutex_t *forks;

	forks = ph_info->common->forks;
	if (ph_info->left == ph_info->right)
		return (false);
	if (ph_info->id % 2)
		pthread_mutex_lock(&forks[ph_info->left]);
	else
	{
		pthread_mutex_lock(&forks[ph_info->right]);
		usleep(300);
	}
	print_log(ph_info, TAKEN_FORK);
	if (ph_info->id % 2)
	{
		usleep(300);
		pthread_mutex_lock(&forks[ph_info->right]);
	}
	else
		pthread_mutex_lock(&forks[ph_info->left]);
	print_log(ph_info, TAKEN_FORK);
	return (true);
}

void ph_drop_forks(const t_philo_info *ph_info)
{
	pthread_mutex_t *forks;

	forks = ph_info->common->forks;
	pthread_mutex_unlock(&forks[ph_info->left]);
//	if (ph_info->id % 2)
		usleep(200);
	pthread_mutex_unlock(&forks[ph_info->right]);
}

void	ph_eat(t_philo_info *ph_info)
{
	set_last_meal_time(ph_info, get_time());
	print_log(ph_info, EATING);
	if (ph_info->left_meal_cnt >= 1)
	{
		ph_info->left_meal_cnt--;
		if (ph_info->left_meal_cnt == 0)
			increment_satisfied_philo(ph_info->common);
	}
	ph_wait(ph_info->common->time_to_eat / 1000);
}

void	ph_sleep(const t_philo_info *ph_info)
{
	print_log(ph_info, SLEEPING);
	ph_wait(ph_info->common->time_to_sleep / 1000);
}

void	ph_think(const t_philo_info *ph_info)
{
	print_log(ph_info, THINKING);
}

void	ph_wait(long wait_time)
{
	const long	start = get_time();

	while (1)
	{
		if (get_time() >= start + wait_time)
			return ;
		usleep(200);
	}
}

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
