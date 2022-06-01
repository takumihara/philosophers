#ifndef PHILO_H
# define PHILO_H

# include <stdbool.h>
# include <signal.h>
# include <sys/semaphore.h>

# define MAX_NUM_OF_PHILO 200
# define MIN_TIME 60

# define SEM_FORKS_ID "/forks"
# define SEM_CHECK_ID "/check"
# define SEM_OUT_ID "/philo_out"

# define ES_ERR -1
# define ES_SATISFIED 0

# define ERR_INVALID_ARGUMENT "error: invalid arguments\n"
# define ERR_PTHREAD_CREATE "error: failed at pthread_create()\n"
# define ERR_PTHREAD_DETACH "error: failed at pthread_detach()\n"
# define ERR_SEM_OPEN "error: failed at sem_open()\n"
# define ERR_SEM_CLOSE "error: failed at sem_close()\n"
# define ERR_SEM_UNLINK "error: failed at sem_unlink()\n"
# define ERR_FORK "error: failed at fork()\n"
# define ERR_KILL "error: failed at kill()\n"

typedef struct s_info {
	int		num_of_philo;
	int		time_to_eat;
	int		time_to_sleep;
	int		time_to_die;
	int		num_of_meal;
	long	start;
	sem_t	*forks;
	sem_t	*sem;
	sem_t	*sem_out;
}	t_info;

typedef struct s_philo_info {
	int		id;
	long	last_meal_time;
	int		left_meal_cnt;
	bool	is_starved;
	bool	simulation_finished;
	t_info	*common;
}	t_philo_info;

typedef enum e_philo_status {
	TAKEN_FORK,
	EATING,
	SLEEPING,
	THINKING,
}	t_philo_status;

bool	init_program(int argc, char **argv, t_info *info);
void	init_philos(t_info *info, pid_t *philos);
void	init_philo_info(t_philo_info *ph_info, t_info *info, int id);

// philo.c
int		philosopher(t_philo_info *ph_info);

// philo_utils.c
int		calc_interval(const t_philo_info *ph_info);
bool	is_starved(const t_philo_info *ph_info);
void	print_log(const t_philo_info *info, t_philo_status status);
void	set_last_meal_time(t_philo_info *ph_info, long last_meal_time);
long	get_last_meal_time(t_philo_info *ph_info);

// monitor.c
void	*monitor(void *arg);

// ph_grab_forks.c
bool	ph_grab_forks(const t_philo_info *ph_info, bool *first_forks);

// destroy.c
bool	destroy_program(t_info *info, pid_t *philos, int num_of_philo);

#endif //PHILO_H
