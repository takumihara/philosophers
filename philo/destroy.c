#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

#include "include/philo.h"
#include "include/utils.h"

bool	destroy_program(t_info *info, pthread_t *philos, int num_of_philo)
{
	int	i;

	i = -1;
	while (++i < num_of_philo)
	{
		if (pthread_join(philos[i], NULL) != 0)
		{
			ft_putstr_fd(ERR_PTHREAD_JOIN, STDERR_FILENO);
			return (false);
		}
	}
	if (!destroy_mutexes(&info->mutex, 1)
		|| !destroy_mutexes(info->forks, info->num_of_philo))
		return (false);
	return (true);
}

bool	destroy_mutexes(pthread_mutex_t *mutexes, int mutex_num)
{
	int	i;

	i = -1;
	while (++i < mutex_num)
	{
		if (pthread_mutex_destroy(&mutexes[i]) != 0)
		{
			ft_putstr_fd(ERR_MUTEX_DESTROY, STDERR_FILENO);
			return (false);
		}
	}
	return (true);
}
