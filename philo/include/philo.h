#ifndef PHILO_H
# define PHILO_H

# include <stdbool.h>

# define MAX_NUM_OF_PHILO 200
# define MIN_MSEC 60

# define ERR_INVALID_ARGUMENT "error: invalid arguments\n"
# define ERR_PTHREAD_CREATE "error: failed at pthread_create()\n"
# define ERR_MUTEX_INIT "error: failed at pthread_mutex_init()\n"
# define ERR_MUTEX_DESTROY "error: failed at pthread_mutex_destroy()\n"
# define ERR_PTHREAD_JOIN "error: failed at pthread_join()\n"

typedef struct s_info {
	int				num_of_philo;
	long long		time_to_eat;
	long long		time_to_sleep;
	long long		time_to_die;
	int				num_of_meal;
	long long		time_to_think;
	long long		start;
	pthread_mutex_t	forks[MAX_NUM_OF_PHILO];
	pthread_mutex_t	mutex;
	bool			simulation_finished;
	int				satisfied_philo;
}	t_info;

typedef struct s_philo_info {
	int			id;
	int			left;
	int			right;
	long long	last_meal_time;
	int			left_meal_cnt;
	t_info		*common;
}	t_philo_info;

typedef enum e_philo_status {
	TAKEN_FORK,
	EATING,
	SLEEPING,
	THINKING,
	DIED,
}	t_philo_status;

bool		init_program(
				int argc, char **argv, t_info *info, t_philo_info *ph_info);
bool		init_philos(t_info *info, t_philo_info *ph_info, pthread_t *philos);
bool		init_mutexes(pthread_mutex_t *mutexes, int mutex_num);
void		init_philo_info(t_philo_info *ph_info, t_info *info, int id);

// philo.c
void		*do_philo(void *arg_);

// philo_utils.c
long long	calc_time_to_think(const t_info *info);
bool		is_simulation_finished(const t_philo_info *ph_info);
void		print_log(const t_philo_info *info, t_philo_status status);
void		set_last_meal_time(t_philo_info *ph_info, long long last_meal_time);
long long	get_last_meal_time(t_philo_info *ph_info);

// monitor.c
void		monitor(t_info *info, t_philo_info *ph_info);
void		increment_satisfied_philo(t_info *info);

// ph_grab_forks.c
bool		ph_grab_forks(const t_philo_info *ph_info, bool *is_first_fork);

// destroy.c
bool		destroy_program(t_info *info, pthread_t *philos, int num_of_philo);
bool		destroy_mutexes(pthread_mutex_t *mutexes, int mutex_num);

#endif //PHILO_H
