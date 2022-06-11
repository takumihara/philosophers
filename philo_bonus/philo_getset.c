#include <stdbool.h>
#include <stdio.h>

#include "include/philo.h"

void	set_last_meal_time(t_philo_info *ph_info, long long last_meal_time)
{
	sem_wait(ph_info->common->sem);
	ph_info->last_meal_time = last_meal_time;
	sem_post(ph_info->common->sem);
}

long long	get_last_meal_time(const t_philo_info *ph_info)
{
	long long	last_meal_time;

	sem_wait(ph_info->common->sem);
	last_meal_time = ph_info->last_meal_time;
	sem_post(ph_info->common->sem);
	return (last_meal_time);
}

void	set_simulation_finished(t_philo_info *ph_info, bool simulation_finished)
{
	sem_wait(ph_info->common->sem);
	ph_info->simulation_finished = simulation_finished;
	sem_post(ph_info->common->sem);
}

bool	get_simulation_finished(const t_philo_info *ph_info)
{
	bool	simulation_finished;

	sem_wait(ph_info->common->sem);
	simulation_finished = ph_info->simulation_finished;
	sem_post(ph_info->common->sem);
	return (simulation_finished);
}

