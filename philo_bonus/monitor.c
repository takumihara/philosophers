#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "include/philo.h"
#include "include/utils.h"

static void	*monitor(void *arg);
static bool	check_starvation(t_philo_info *ph_info);
static bool check_simulation_finished(t_philo_info *ph_info);


pthread_t	prep_monitor(t_philo_info *ph_info)
{
	pthread_t	pthread;

	if (pthread_create(&pthread, NULL, monitor, ph_info) != 0)
	{
		printf(ERR_PTHREAD_CREATE);
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
		if (check_simulation_finished(ph_info))
			break ;
		usleep(500);
	}
	return (NULL);
}

bool	check_starvation(t_philo_info *ph_info)
{
	int		i;
	bool	is_starved;
	long long	last_meal_time;
	t_info	*info;

	info = ph_info->common;
	i = 0;
	is_starved = false;
//	printf("%d in the loop %lld \n", ph_info->id, get_usec() - ph_info->common->start);
	last_meal_time = get_last_meal_time(&ph_info[i]);
	if (get_usec() - last_meal_time >= info->time_to_die)
	{
//		printf("%d found out %lld \n", ph_info->id, get_usec() - ph_info->common->start);
		sem_wait(info->sem);
		ph_info->is_starved = true;
		ph_info->simulation_finished = true;
		is_starved = true;
		sem_post(info->sem);
		print_log(ph_info, DIED);
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