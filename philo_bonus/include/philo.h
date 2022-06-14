#ifndef PHILO_H
# define PHILO_H

# include <stdbool.h>
# include <signal.h>
# include <semaphore.h>

# define MAX_NUM_OF_PHILO 200
# define MIN_MSEC 60

# define SEM_FORKS_ID "/philo_forks"
# define SEM_ACCESS_ID "/philo_access"
# define SEM_OUT_ID "/philo_out"

# define ES_ERR 0
# define ES_SATISFIED 1
# define ES_STARVED 2

# define ERR_INVALID_ARGUMENT "error: invalid arguments\n"
# define ERR_PTHREAD_CREATE "error: failed at pthread_create()\n"
# define ERR_PTHREAD_JOIN "error: failed at pthread_join()\n"
# define ERR_SEM_OPEN "error: failed at sem_open()\n"
# define ERR_SEM_CLOSE "error: failed at sem_close()\n"
# define ERR_SEM_UNLINK "error: failed at sem_unlink()\n"
# define ERR_FORK "error: failed at fork()\n"
# define ERR_KILL "error: failed at kill()\n"

typedef struct s_info {
	int			num_of_philo;
	int			time_to_eat;
	int			time_to_sleep;
	int			time_to_die;
	int			num_of_meal;
	long long	start;
	sem_t		*forks;
	sem_t		*sem_access;
	sem_t		*sem_out;
}	t_info;

typedef struct s_philo_info {
	int			id;
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

// init.c
bool		init_program(int argc, char **argv, t_info *info);
void		init_philos(t_info *info, pid_t *philos);

// philo.c
int			do_philo(t_philo_info *ph_info);

// philo_utils.c
int			calc_interval(const t_philo_info *ph_info);
void		print_log(const t_philo_info *info, t_philo_status status);

// philo_getset.c
void		set_last_meal_time(t_philo_info *ph_info, long long last_meal_time);
long long	get_last_meal_time(const t_philo_info *ph_info);
void		dec_left_meal_cnt(t_philo_info *ph_info);
int			get_left_meal_cnt(const t_philo_info *ph_info);

// monitor.c
pthread_t	init_monitor(t_philo_info *ph_info);

// ph_grab_forks.c
bool		ph_grab_forks(const t_philo_info *ph_info, bool *is_first_fork);

// destroy.c
bool		destroy_program(t_info *info, pid_t *philos, int num_of_philo);

#endif //PHILO_H
