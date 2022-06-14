#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "include/philo.h"
#include "include/utils.h"
#include "include/unwrap.h"

static void	*monitor(void *arg);
static bool	check_starvation(const t_philo_info *ph_info);

pthread_t	init_monitor(t_philo_info *ph_info)
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
	const t_philo_info	*ph_info = arg;

	while (true)
	{
		if (check_starvation(ph_info))
			exit(ES_STARVED);
		if (get_left_meal_cnt(ph_info) == 0)
			return (NULL);
		usleep(500);
	}
}

bool	check_starvation(const t_philo_info *ph_info)
{
	long long	last_meal_time;
	t_info		*info;

	info = ph_info->common;
	last_meal_time = get_last_meal_time(ph_info);
	if (get_usec() - last_meal_time >= info->time_to_die)
	{
		print_log(ph_info, DIED);
		return (true);
	}
	return (false);
}
