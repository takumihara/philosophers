#include <pthread.h>
#include <stdlib.h>

#include "include/philo.h"

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
