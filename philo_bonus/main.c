#include <stdlib.h>
#include <signal.h>
#include <printf.h>

#include "include/philo.h"
#include "include/utils.h"

static void	check_philo_status(t_info *info, pid_t *philos);

int	main(int argc, char **argv)
{
	t_info			info;
	pid_t			philos[MAX_NUM_OF_PHILO];

	if (!init_program(argc, argv, &info))
		return (EXIT_FAILURE);
	init_philos(&info, philos);
	check_philo_status(&info, philos);
	return (EXIT_SUCCESS);
}

static void	check_philo_status(t_info *info, pid_t *philos)
{
	int				status;
	volatile int	i;
	bool			someone_died;
	int				exit_status;

	i = 0;
	someone_died = false;
	while (i++ < info->num_of_philo)
	{
		waitpid(-1, &status, 0);
		exit_status = WEXITSTATUS(status);
		printf("%d %d \n", i, exit_status);
		if (!someone_died && exit_status != ES_SATISFIED && exit_status != ES_ERR)
		{
			sem_wait(info->sem);
			someone_died = true;
			printf("%ld %d died\n", get_time() - info->start, exit_status);
			if (!destroy_program(info, philos, info->num_of_philo))
				return ;
		}
	}
}
