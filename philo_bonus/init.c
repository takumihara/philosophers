#include <string.h>
#include <stdio.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#include "include/philo.h"
#include "include/utils.h"

static bool	parse_args(int argc, char **argv, t_info *info);
static int	do_philo(t_philo_info *ph_info);

bool	init_program(int argc, char **argv, t_info *info)
{
	if (!parse_args(argc, argv, info))
		return (false);
	sem_unlink(SEM_CHECK_ID);
	sem_unlink(SEM_FORKS_ID);
	sem_unlink(SEM_OUT_ID);
	info->sem = sem_open(SEM_CHECK_ID, O_CREAT | O_EXCL, 0600, 1);
	info->forks = sem_open(
			SEM_FORKS_ID, O_CREAT | O_EXCL, 0600, info->num_of_philo);
	info->sem_out = sem_open(SEM_OUT_ID, O_CREAT | O_EXCL, 0600, 1);
	if (info->sem == SEM_FAILED
		|| info->forks == SEM_FAILED
		|| info->sem_out == SEM_FAILED)
	{
		printf(ERR_SEM_OPEN);
		destroy_program(info, NULL, 0);
		return (false);
	}
	return (true);
}

bool	parse_args(int argc, char **argv, t_info *info)
{
	info->num_of_meal = -1;
	if (!((argc == 5 || argc == 6)
			&& atoi_strict(argv[1], &info->num_of_philo)
			&& info->num_of_philo >= 1
			&& info->num_of_philo <= MAX_NUM_OF_PHILO
			&& atoi_strict(argv[2], &info->time_to_die)
			&& info->time_to_die >= MIN_MSEC
			&& atoi_strict(argv[3], &info->time_to_eat)
			&& info->time_to_eat >= MIN_MSEC
			&& atoi_strict(argv[4], &info->time_to_sleep)
			&& info->time_to_sleep >= MIN_MSEC
			&& (argc == 5 || atoi_strict(argv[5], &info->num_of_meal))))
	{
		printf(ERR_INVALID_ARGUMENT);
		return (false);
	}
	info->time_to_die *= 1000;
	info->time_to_eat *= 1000;
	info->time_to_sleep *= 1000;
	return (true);
}

void	init_philos(t_info *info, pid_t *philos)
{
	int				i;
	pid_t			pid;
	t_philo_info	ph_info;

	i = -1;
	info->start = get_usec();
	while (++i < info->num_of_philo)
	{
		init_philo_info(&ph_info, info, i + 1);
		pid = fork();
		if (pid == 0)
			exit(do_philo(&ph_info));
		else if (pid < 0)
		{
			printf(ERR_FORK);
			destroy_program(info, philos, i);
			exit(EXIT_FAILURE);
		}
		else
			philos[i] = pid;
	}
}

int	do_philo(t_philo_info *ph_info)
{
	int			res;
	pthread_t	pthread;

	if (ph_info->left_meal_cnt == 0)
		return (ES_SATISFIED);
	pthread = prep_monitor(ph_info);
	res = ph_loop(ph_info);
	if (res == ES_SATISFIED)
	{
		sem_wait(ph_info->common->sem);
		ph_info->simulation_finished = true;
		sem_post(ph_info->common->sem);
	}
	if (pthread_join(pthread, NULL) != 0)
		printf(ERR_PTHREAD_JOIN);
	return (res);
}

void	init_philo_info(t_philo_info *ph_info, t_info *info, int id)
{
	ph_info->id = id;
	ph_info->last_meal_time = info->start;
	ph_info->left_meal_cnt = info->num_of_meal;
	ph_info->simulation_finished = false;
	ph_info->is_starved = false;
	ph_info->common = info;
}
