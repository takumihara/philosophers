#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

#include "include/philo.h"
#include "include/utils.h"

int main(int argc, char **argv)
{
	t_info	info;
	int		i;
	pthread_t		*philos;
	t_philo_info	*args;

	if (argc != 5 && argc != 6)
	{
		printf("argument error \n");
		return (1);
	}
	pthread_mutex_init(&info.mutex, NULL);
	info.num_of_philo = ft_atoi(argv[1]);
	info.time_to_die = ft_atoi(argv[2]);
	info.time_to_eat = ft_atoi(argv[3]) * 1000;
	info.time_to_sleep = ft_atoi(argv[4]) * 1000;
	if (argc == 6)
		info.num_of_meal = ft_atoi(argv[5]);
	else
		info.num_of_meal = -1;
	info.forks = init_fork(info.num_of_philo);
	if (!info.forks)
		return (EXIT_FAILURE);
	philos = init_philo(info.num_of_philo);
	if (!philos)
		return (EXIT_FAILURE);
	args = malloc(sizeof(*args) * info.num_of_philo);
	if (!args)
		return (EXIT_FAILURE);
	i = -1;
	info.start = get_time();
	while (++i < info.num_of_philo)
	{
		init_philo_info(&args[i], &info, i + 1, info.start);
		pthread_create(&philos[i], NULL, philosopher, (void *)&args[i]);
	}
	while (1)
	{
		if (is_starved(&info, args))
			break;
		if (all_satisfied(&info))
			break;
		usleep(200);
	}
	i = -1;
	while (++i < info.num_of_philo)
		pthread_join(philos[i], NULL);
	return 0;
}

void	print_log(const t_philo_info *info, t_philo_status status)
{
	const long time_stamp = get_time() - info->common->start;

	pthread_mutex_lock(&info->common->mutex);
	if (status != DIED && info->common->simulation_finished)
	{
		pthread_mutex_unlock(&info->common->mutex);
		return;
	}
	if (status == TAKEN_FORK)
		printf("%ld %d has taken a fork\n", time_stamp, info->id);
	else if (status == EATING)
		printf("%ld %d is eating\n", time_stamp, info->id);
	else if (status == SLEEPING)
		printf("%ld %d is sleeping\n", time_stamp, info->id);
	else if (status == THINKING)
		printf("%ld %d is thinking\n", time_stamp, info->id);
	else if (status == DIED)
		printf("%ld %d died\n", time_stamp, info->id);
	pthread_mutex_unlock(&info->common->mutex);
}
