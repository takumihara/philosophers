#include <stdbool.h>
#include <printf.h>

#include "include/philo.h"
#include "include/utils.h"
#include "include/unwrap.h"

void	ph_drop_forks(const t_philo_info *ph_info);
void	ph_eat(t_philo_info *ph_info);
void	ph_sleep(const t_philo_info *ph_info);
void	ph_think(const t_philo_info *ph_info);

void	ph_loop(t_philo_info *ph_info)
{
	bool	first_fork;

	first_fork = true;
	while (true)
	{
		if (!ph_grab_forks(ph_info, &first_fork))
			continue ;
		ph_eat(ph_info);
		ph_drop_forks(ph_info);
		if (get_left_meal_cnt(ph_info) == 0)
			return ;
		ph_sleep(ph_info);
		ph_think(ph_info);
	}
}

void	ph_drop_forks(const t_philo_info *ph_info)
{
	sem_post_unwrap(ph_info->common->forks);
	sem_post_unwrap(ph_info->common->forks);
}

void	ph_eat(t_philo_info *ph_info)
{
	set_last_meal_time(ph_info, get_usec());
	print_log(ph_info, EATING);
	if (ph_info->left_meal_cnt >= 1)
		dec_left_meal_cnt(ph_info);
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
