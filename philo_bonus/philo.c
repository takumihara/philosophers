#include <stdbool.h>

#include "include/philo.h"
#include "include/utils.h"

void	ph_drop_forks(const t_philo_info *ph_info);
bool	ph_eat(t_philo_info *ph_info);
void	ph_sleep(const t_philo_info *ph_info);
void	ph_think(const t_philo_info *ph_info);

int	ph_loop(t_philo_info *ph_info)
{
	bool	first_forks;
	bool	satisfied;

	first_forks = true;
	satisfied = false;
	while (true)
	{
		if (is_starved(ph_info))
			return (ES_STARVED);
		if (!ph_grab_forks(ph_info, &first_forks))
			continue ;
		satisfied = ph_eat(ph_info);
		ph_drop_forks(ph_info);
		if (satisfied)
			return (ES_SATISFIED);
		ph_sleep(ph_info);
		ph_think(ph_info);
	}
}

void	ph_drop_forks(const t_philo_info *ph_info)
{
	sem_post(ph_info->common->forks);
	sem_post(ph_info->common->forks);
}

bool	ph_eat(t_philo_info *ph_info)
{
	bool	satisfied;

	satisfied = false;
	set_last_meal_time(ph_info, get_usec());
	print_log(ph_info, EATING);
	if (ph_info->left_meal_cnt >= 1)
	{
		ph_info->left_meal_cnt--;
		if (ph_info->left_meal_cnt == 0)
			satisfied = true;
	}
	msleep_precise(get_usec(), ph_info->common->time_to_eat);
	return (satisfied);
}

void	ph_sleep(const t_philo_info *ph_info)
{
	print_log(ph_info, SLEEPING);
	msleep_precise(get_usec(), ph_info->common->time_to_sleep);
}

void	ph_think(const t_philo_info *ph_info)
{
	print_log(ph_info, THINKING);
}
