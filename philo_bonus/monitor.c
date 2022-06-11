#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "include/philo.h"
#include "include/utils.h"

static void	*monitor(void *arg);
static bool	check_starvation(t_philo_info *ph_info);

pthread_t	prep_monitor(t_philo_info *ph_info)
{
	pthread_t	pthread;

	if (pthread_create(&pthread, NULL, monitor, ph_info) != 0)
	{
		ft_putstr_fd(ERR_PTHREAD_CREATE, STDERR_FILENO);
		exit(ES_ERR);
	}
	return (pthread);
}

void	*monitor(void *arg)
{
	t_philo_info	*ph_info;

	ph_info = arg;
	while (true)
	{
		if (check_starvation(ph_info))
			break ;
		if (get_simulation_finished(ph_info))
			break ;
		usleep(500);
	}
	return (NULL);
}

bool	check_starvation(t_philo_info *ph_info)
{
	bool		is_starved;
	long long	last_meal_time;
	t_info		*info;

	info = ph_info->common;
	is_starved = false;
	last_meal_time = get_last_meal_time(ph_info);
	if (get_usec() - last_meal_time >= info->time_to_die)
	{
		sem_wait(info->sem);
		ph_info->is_starved = true;
		ph_info->simulation_finished = true;
		is_starved = true;
		sem_post(info->sem);
		print_log(ph_info, DIED);
	}
	return (is_starved);
}
