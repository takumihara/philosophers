#include "include/philo.h"
#include "include/unwrap.h"

void	set_last_meal_time(t_philo_info *ph_info, long long last_meal_time)
{
	sem_wait_unwrap(ph_info->common->sem_access);
	ph_info->last_meal_time = last_meal_time;
	sem_post_unwrap(ph_info->common->sem_access);
}

long long	get_last_meal_time(const t_philo_info *ph_info)
{
	long long	last_meal_time;

	sem_wait_unwrap(ph_info->common->sem_access);
	last_meal_time = ph_info->last_meal_time;
	sem_post_unwrap(ph_info->common->sem_access);
	return (last_meal_time);
}

void	decrement_left_meal_cnt(t_philo_info *ph_info)
{
	sem_wait_unwrap(ph_info->common->sem_access);
	ph_info->left_meal_cnt -= 1;
	sem_post_unwrap(ph_info->common->sem_access);
}

int	get_left_meal_cnt(const t_philo_info *ph_info)
{
	int	left_meal_cnt;

	sem_wait_unwrap(ph_info->common->sem_access);
	left_meal_cnt = ph_info->left_meal_cnt;
	sem_post_unwrap(ph_info->common->sem_access);
	return (left_meal_cnt);
}
