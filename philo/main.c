#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "include/philo.h"
#include "include/utils.h"

void	monitor(t_info *info, t_philo_info *ph_info);

int	main(int argc, char **argv)
{
	t_info			info;
	pthread_t		philos[MAX_NUM_OF_PHILO];
	t_philo_info	ph_info[MAX_NUM_OF_PHILO];

	if (!init_program(argc, argv, &info, ph_info))
		return (EXIT_FAILURE);
	if (!init_philos(&info, ph_info, philos))
		return (EXIT_FAILURE);
	monitor(&info, ph_info);
	if (!destroy_program(&info, philos, info.num_of_philo))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

void	monitor(t_info *info, t_philo_info *ph_info)
{
	while (true)
	{
		if (is_starved(info, ph_info))
			return ;
		if (all_satisfied(info))
			return ;
		usleep(500);
	}
}

void	print_log(const t_philo_info *info, t_philo_status status)
{
	const long	timestamp = get_time() - info->common->start;

	pthread_mutex_lock(&info->common->mutex);
	if (status != DIED && info->common->simulation_finished)
	{
		pthread_mutex_unlock(&info->common->mutex);
		return ;
	}
	if (status == TAKEN_FORK)
		printf("%ld %d has taken a fork\n", timestamp, info->id);
	else if (status == EATING)
		printf("%ld %d is eating\n", timestamp, info->id);
	else if (status == SLEEPING)
		printf("%ld %d is sleeping\n", timestamp, info->id);
	else if (status == THINKING)
		printf("%ld %d is thinking\n", timestamp, info->id);
	else if (status == DIED)
		printf("%ld %d died\n", timestamp, info->id);
	pthread_mutex_unlock(&info->common->mutex);
}
