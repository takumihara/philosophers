#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "include/philo.h"
#include "include/utils.h"

pthread_t	prep_monitor(t_philo_info *ph_info);
void	ph_drop_forks(const t_philo_info *ph_info);
bool	ph_eat(t_philo_info *ph_info);
void	ph_sleep(const t_philo_info *ph_info);
void	ph_think(const t_philo_info *ph_info);

int		philosopher(t_philo_info *ph_info)
{
	bool	first_forks;
	bool	satisfied;
	int		res;
	pthread_t	pthread;

	if (ph_info->left_meal_cnt == 0)
		return (ES_SATISFIED);
	first_forks = true;
	satisfied = false;
	pthread = prep_monitor(ph_info);
	while (true)
	{
		if (is_starved(ph_info))
		{
			res = ES_STARVED;
			break ;
		}
		if (!ph_grab_forks(ph_info, &first_forks))
			continue ;
		satisfied = ph_eat(ph_info);
		ph_drop_forks(ph_info);
		if (satisfied)
		{
			sem_wait(ph_info->common->sem);
			ph_info->simulation_finished = true;
			sem_post(ph_info->common->sem);
			res = ES_SATISFIED;
			break ;
		}
		ph_sleep(ph_info);
		ph_think(ph_info);
	}
	if (pthread_join(pthread, NULL) != 0)
		printf(ERR_PTHREAD_JOIN);
	return (res);
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
	set_last_meal_time(ph_info, get_time());
	print_log(ph_info, EATING);
	if (ph_info->left_meal_cnt >= 1)
	{
		ph_info->left_meal_cnt--;
		if (ph_info->left_meal_cnt == 0)
			satisfied = true;
	}
	sleep_precisely(get_time(), ph_info->common->time_to_eat);
	return (satisfied);
}

void	ph_sleep(const t_philo_info *ph_info)
{
	print_log(ph_info, SLEEPING);
	sleep_precisely(get_time(), ph_info->common->time_to_sleep);
}

void	ph_think(const t_philo_info *ph_info)
{
	print_log(ph_info, THINKING);
}
