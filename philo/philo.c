#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <printf.h>

#include "include/philo.h"
#include "include/utils.h"


void	ph_drop_forks(const t_philo_info *ph_info);
void	ph_eat(t_philo_info *ph_info);
void	ph_sleep(const t_philo_info *ph_info);
void	ph_think(const t_philo_info *ph_info);

bool	is_simulation_finished(const t_philo_info *ph_info);
int		calc_interval(const t_philo_info *ph_info);

void *philosopher(void *arg_)
{
	t_philo_info 	*ph_info = (t_philo_info*)arg_;
	bool			first_forks;

	first_forks = true;
	while (1)
	{
		if (is_simulation_finished(ph_info))
			return (NULL);
		if (!ph_grab_forks(ph_info, &first_forks))
			continue;
		ph_eat(ph_info);
		ph_drop_forks(ph_info);
		ph_sleep(ph_info);
		ph_think(ph_info);
	}
}

void ph_drop_forks(const t_philo_info *ph_info)
{
	pthread_mutex_t *forks;

	forks = ph_info->common->forks;
	pthread_mutex_unlock(&forks[ph_info->left]);
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
	ph_wait(get_time(), ph_info->common->time_to_eat / 1000);
}

void	ph_sleep(const t_philo_info *ph_info)
{
	print_log(ph_info, SLEEPING);
	ph_wait(get_time(), ph_info->common->time_to_sleep / 1000);
}

void	ph_think(const t_philo_info *ph_info)
{
	print_log(ph_info, THINKING);
}

void	ph_wait(long start, long wait_time)
{
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
