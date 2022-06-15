#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>

#include "include/philo.h"
#include "include/utils.h"
#include "include/unwrap.h"

void	ph_drop_forks(const t_philo_info *ph_info);
void	ph_eat(t_philo_info *ph_info);
void	ph_sleep(const t_philo_info *ph_info);
void	ph_think(const t_philo_info *ph_info);

int	do_philo(t_philo_info *ph_info)
{
	bool		is_first_fork;
	pthread_t	pthread_monitor;

	if (ph_info->left_meal_cnt == 0)
		return (ES_SATISFIED);
	pthread_monitor = init_monitor(ph_info);
	is_first_fork = true;
	while (true)
	{
		if (!ph_grab_forks(ph_info, &is_first_fork))
			continue ;
		ph_eat(ph_info);
		ph_drop_forks(ph_info);
		if (get_left_meal_cnt(ph_info) == 0)
			break ;
		ph_sleep(ph_info);
		ph_think(ph_info);
	}
	if (pthread_join(pthread_monitor, NULL) != 0)
		ft_putstr_fd(ERR_PTHREAD_JOIN, STDERR_FILENO);
	return (ES_SATISFIED);
}

void	ph_drop_forks(const t_philo_info *ph_info)
{
	sem_post_unwrap(ph_info->common->forks);
	sem_post_unwrap(ph_info->common->forks);
}

void	ph_eat(t_philo_info *ph_info)
{
	set_last_meal_time(ph_info, get_usec());
	print_log(ph_info, EATING);
	if (ph_info->left_meal_cnt >= 1)
		decrement_left_meal_cnt(ph_info);
	usleep_until(get_usec() + ph_info->common->time_to_eat);
}

void	ph_sleep(const t_philo_info *ph_info)
{
	print_log(ph_info, SLEEPING);
	usleep_until(get_usec() + ph_info->common->time_to_sleep);
}

void	ph_think(const t_philo_info *ph_info)
{
	print_log(ph_info, THINKING);
}
