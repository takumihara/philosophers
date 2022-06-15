#include <pthread.h>
#include <stdbool.h>
#include <string.h>

#include "include/philo.h"
#include "include/utils.h"
#include "include/unwrap.h"

static void	ph_drop_forks(const t_philo_info *ph_info);
static void	ph_eat(t_philo_info *ph_info);
static void	ph_sleep(const t_philo_info *ph_info);
static void	ph_think(const t_philo_info *ph_info);

void	*do_philo(void *arg)
{
	t_philo_info	*ph_info;
	bool			is_first_fork;

	ph_info = (t_philo_info *)arg;
	is_first_fork = true;
	while (!is_simulation_finished(ph_info))
	{
		if (!ph_grab_forks(ph_info, &is_first_fork))
			continue ;
		ph_eat(ph_info);
		ph_drop_forks(ph_info);
		ph_sleep(ph_info);
		ph_think(ph_info);
	}
	return (NULL);
}

void	ph_drop_forks(const t_philo_info *ph_info)
{
	pthread_mutex_t	*forks;

	forks = ph_info->common->forks;
	pthread_mutex_unlock_unwrap(&forks[ph_info->left]);
	pthread_mutex_unlock_unwrap(&forks[ph_info->right]);
}

void	ph_eat(t_philo_info *ph_info)
{
	set_last_meal_time(ph_info, get_usec());
	print_log(ph_info, EATING);
	if (ph_info->left_meal_cnt >= 1)
	{
		ph_info->left_meal_cnt--;
		if (ph_info->left_meal_cnt == 0)
			increment_satisfied_philo(ph_info->common);
	}
	usleep_until(get_usec() + ph_info->common->time_to_eat);
}

void	ph_sleep(const t_philo_info *ph_info)
{
	print_log(ph_info, SLEEPING);
	usleep_until(get_usec() + ph_info->common->time_to_sleep);
}

void	ph_think(const t_philo_info *ph_info)
{
	print_log(ph_info, THINKING);
}
