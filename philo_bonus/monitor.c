#include <pthread/pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <printf.h>

#include "include/philo.h"
#include "include/utils.h"

static bool	check_starvation(t_info *info, t_philo_info *ph_info);
static bool check_simulation_finished(t_philo_info *ph_info);

void	*monitor(void *arg)
{
	t_philo_info	*ph_info;

	ph_info = arg;
	while (true)
	{
		if (check_starvation(ph_info->common, ph_info))
			break ;
		if (check_simulation_finished(ph_info))
			break ;
		usleep(500);
	}
	return (NULL);
}

bool	check_starvation(t_info *info, t_philo_info *ph_info)
{
	int		i;
	bool	is_starved;
	long	last_meal_time;

	i = 0;
	is_starved = false;
	last_meal_time = get_last_meal_time(&ph_info[i]);
	if (get_time() - last_meal_time >= info->time_to_die)
	{
		sem_wait(info->sem);
		ph_info->is_starved = true;
		is_starved = true;
		sem_post(info->sem);
	}
	return (is_starved);
}

static bool check_simulation_finished(t_philo_info *ph_info)
{
	bool	simulation_finished;

	sem_wait(ph_info->common->sem);
	simulation_finished = ph_info->simulation_finished;
	sem_post(ph_info->common->sem);
	return (simulation_finished);
}