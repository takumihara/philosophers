#include <stdbool.h>
#include <stdio.h>

#include "include/philo.h"
#include "include/unwrap.h"

void	set_last_meal_time(t_philo_info *ph_info, long long last_meal_time)
{
	sem_wait_unwrap(ph_info->common->sem);
	ph_info->last_meal_time = last_meal_time;
	sem_post_unwrap(ph_info->common->sem);
}

long long	get_last_meal_time(const t_philo_info *ph_info)
{
	long long	last_meal_time;

	sem_wait_unwrap(ph_info->common->sem);
	last_meal_time = ph_info->last_meal_time;
	sem_post_unwrap(ph_info->common->sem);
	return (last_meal_time);
}

void	dec_left_meal_cnt(t_philo_info *ph_info)
{
	sem_wait_unwrap(ph_info->common->sem);
	ph_info->left_meal_cnt -= 1;
	sem_post_unwrap(ph_info->common->sem);
}

int	get_left_meal_cnt(const t_philo_info *ph_info)
{
	int	left_meal_cnt;

	sem_wait_unwrap(ph_info->common->sem);
	left_meal_cnt = ph_info->left_meal_cnt;
	sem_post_unwrap(ph_info->common->sem);
	return (left_meal_cnt);
}

bool	get_is_starved(const t_philo_info *ph_info)
{
	bool	is_starved;

	sem_wait_unwrap(ph_info->common->sem);
	is_starved = ph_info->is_starved;
	sem_post_unwrap(ph_info->common->sem);
	return (is_starved);
}
