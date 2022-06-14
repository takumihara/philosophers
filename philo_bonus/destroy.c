#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>

#include "include/philo.h"
#include "include/utils.h"

bool	destroy_philos(pid_t *philos, int num_of_philo)
{
	int	i;

	if (!philos)
		return (true);
	i = -1;
	while (++i < num_of_philo)
	{
		if (kill(philos[i], SIGINT) != 0)
		{
			if (errno == ESRCH)
				continue ;
			ft_putstr_fd(ERR_KILL, STDERR_FILENO);
			return (false);
		}
	}
	return (true);
}

bool	destroy_sem(sem_t *sem, char *sem_id)
{
	if (sem == SEM_FAILED)
		return (true);
	if (sem_close(sem) != 0)
	{
		ft_putstr_fd(ERR_SEM_CLOSE, STDERR_FILENO);
		return (false);
	}
	if (sem_unlink(sem_id) != 0)
	{
		ft_putstr_fd(ERR_SEM_UNLINK, STDERR_FILENO);
		return (false);
	}
	return (true);
}

bool	destroy_program(t_info *info, pid_t *philos, int num_of_philo)
{
	bool	res;

	res = destroy_philos(philos, num_of_philo);
	res &= destroy_sem(info->sem_access, SEM_ACCESS_ID);
	res &= destroy_sem(info->forks, SEM_FORKS_ID);
	res &= destroy_sem(info->sem_out, SEM_OUT_ID);
	return (res);
}
