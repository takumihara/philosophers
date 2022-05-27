#ifndef PHILO_H
#define PHILO_H

#include <stdbool.h>

typedef struct s_info {
	// immutable
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
	int		num_of_philo;
	int		num_of_meal;
	long	start;
	// mutable
	pthread_mutex_t	*forks;
	pthread_mutex_t mutex;
	bool	simulation_finished;
	int		satisfied_philo;
}	t_info;

typedef struct s_philo_info {
	int		id;
	int		left;
	int		right;
	long	last_meal_time;
	int		left_meal_cnt;
	t_info	*common;
}	t_philo_info;

typedef struct s_monitor {
	t_philo_info	*args;
	pthread_t		*philos;
	t_info	*info;
}	t_monitor;

typedef enum e_philo_status {
	TAKEN_FORK,
	EATING,
	SLEEPING,
	THINKING,
	DIED,
}	t_philo_status;

pthread_t	*init_philo(int num_of_philo);
pthread_mutex_t	*init_fork(int num_of_philo);
void	init_philo_info(t_philo_info *arg, t_info *info, int p_id, long start);

void	print_log(const t_philo_info *info, t_philo_status status);

// philo.c
void	*philosopher(void *arg_);

// monitor.c
bool is_starved(t_info *info, t_philo_info *ph_info);
bool all_satisfied(t_info *info);
void	increment_satisfied_philo(t_info *info);
int		get_satisfied_philo(t_info *info);
void	set_last_meal_time(t_philo_info *ph_info, long last_meal_time);


#endif //PHILO_H
