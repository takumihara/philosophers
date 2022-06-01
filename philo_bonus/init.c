#include <pthread.h>
#include <string.h>
#include <printf.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#include "include/philo.h"
#include "include/utils.h"

bool	init_program(int argc, char **argv, t_info *info)
{
	info->num_of_meal = -1;
	if (!((argc == 5 || argc == 6)
			  && atoi_strict(argv[1], &info->num_of_philo)
			  && info->num_of_philo >= 1
			  && info->num_of_philo <= MAX_NUM_OF_PHILO
			  && atoi_strict(argv[2], &info->time_to_die)
			  && info->time_to_die >= MIN_TIME
			  && atoi_strict(argv[3], &info->time_to_eat)
			  && info->time_to_eat >= MIN_TIME
			  && atoi_strict(argv[4], &info->time_to_sleep)
			  && info->time_to_sleep >= MIN_TIME
			  && (argc == 5 || atoi_strict(argv[5], &info->num_of_meal))))
	{
		printf(ERR_INVALID_ARGUMENT);
		return (false);
	}
	// todo: what is only the second failed. should I release the first?
	sem_unlink(SEM_CHECK_ID);
	info->sem = sem_open(SEM_CHECK_ID, O_CREAT | O_EXCL, 0600, 1);
	sem_unlink(SEM_FORKS_ID);
	info->forks = sem_open(SEM_FORKS_ID, O_CREAT | O_EXCL, 0600, info->num_of_philo);
	sem_unlink(SEM_OUT_ID);
	info->sem_out = sem_open(SEM_OUT_ID, O_CREAT | O_EXCL, 0600, 1);
	if (info->sem == SEM_FAILED || info->forks == SEM_FAILED || info->sem_out == SEM_FAILED)
	{
		printf(ERR_SEM_OPEN);
		return (false);
	}
	return (true);
}

void	init_philos(t_info *info, pid_t *philos)
{
	int				i;
	pid_t			pid;
	t_philo_info	ph_info;

	i = -1;
	info->start = get_time();
	while (++i < info->num_of_philo)
	{
		init_philo_info(&ph_info, info, i + 1);
		pid = fork();
		if (pid == 0)
			exit(philosopher(&ph_info));
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

void	init_philo_info(t_philo_info *ph_info, t_info *info, int id)
{
	ph_info->id = id;
	ph_info->last_meal_time = info->start;
	ph_info->left_meal_cnt = info->num_of_meal;
	ph_info->simulation_finished = false;
	ph_info->is_starved = false;
	ph_info->common = info;
}
