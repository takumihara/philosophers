#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

typedef struct s_arg	t_arg;
typedef struct s_info	t_info;

struct s_info {
	pthread_mutex_t	*forks;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
	int		num_of_philo;
	int		num_of_meal;
};

struct s_arg {
	int		p_id;
	long	last_meal_time;
	t_info	*info;
};

typedef struct s_monitor_arg {
	t_arg		*args;
	pthread_t	*philos;
	t_info	*info;
}	t_monitor_arg;

static pthread_t	*init_philo(int num_of_philo);
static pthread_mutex_t	*init_fork(int num_of_philo);
t_arg *init_arg(t_arg *arg, t_info *info, int p_id, long start);
static void		*philosopher(void *arg_);
long    get_time(void);
void grab_fork(int p_id, int fork_id, pthread_mutex_t *fork);
void drop_forks(int p_id, pthread_mutex_t *fork, int num_of_philo);
void *monitor(void *arg_);

int main(int argc, char **argv)
{
	t_info	info;
	int		i;
	long	start;
	pthread_t monitor_thread;
	t_monitor_arg *monitor_arg;


	if (argc != 5 && argc != 6)
	{
		printf("argument error \n");
		return (1);
	}
	monitor_arg = malloc(sizeof(t_monitor_arg));
	if (!monitor_arg)
		return (EXIT_FAILURE);
	info.num_of_philo = atoi(argv[1]);
	info.time_to_die = atoi(argv[2]);
	info.time_to_eat = atoi(argv[3]);
	info.time_to_sleep = atoi(argv[4]);
	if (argc == 6)
		info.num_of_meal = atoi(argv[5]);
	else
		info.num_of_meal = -1;
	info.forks = init_fork(info.num_of_philo);
	if (!info.forks)
		return (EXIT_FAILURE);
	monitor_arg->philos = init_philo(info.num_of_philo);
	if (!monitor_arg->philos)
		return (EXIT_FAILURE);
	monitor_arg->args = malloc(sizeof(*monitor_arg->args) * info.num_of_philo);
	if (!monitor_arg->args)
		return (EXIT_FAILURE);
	i = -1;
	start = get_time();
	while (++i < info.num_of_philo)
	{
		init_arg(&monitor_arg->args[i], &info, i + 1, start);
		pthread_create(&monitor_arg->philos[i], NULL, philosopher, (void *)&monitor_arg->args[i]);
	}
	monitor_arg->info = &info;
	pthread_create(&monitor_thread, NULL, monitor, (void *)&monitor_arg);
	pthread_join(monitor_thread, NULL);
	return (0);
//	i = -1;
//	while (++i < info.num_of_philo)
//		pthread_join(monitor_arg->philos[i], NULL);
	return 0;
}

void *philosopher(void *arg_)
{
	t_arg	*arg;
	int left_chopstick;
	int right_chopstick;

	arg = (t_arg*)arg_;
	left_chopstick = arg->p_id - 1;
	right_chopstick = arg->p_id;
	if (left_chopstick == 0)
		left_chopstick = arg->info->num_of_philo;
	while (arg->info->num_of_meal)
	{
		if (0 < arg->info->num_of_meal)
			arg->info->num_of_meal--;
		if (arg->p_id % 2)
			usleep(200);
		grab_fork(arg->p_id, left_chopstick, arg->info->forks);
		grab_fork(arg->p_id, right_chopstick, arg->info->forks);
		printf("%ld %d is eating\n", get_time(), arg->p_id);
		usleep(arg->info->time_to_eat);
		drop_forks(arg->p_id, arg->info->forks, arg->info->num_of_philo);
		printf("%ld %d is sleeping\n", get_time(), arg->p_id);
		usleep(arg->info->time_to_sleep);
		printf("%ld %d is thinking\n", get_time(), arg->p_id);
	}
	return ((void *)0);
}

void *monitor(void *arg_)
{
	const t_monitor_arg	*monitor = (t_monitor_arg*)arg_;
	long time;
	int i;
//	const int time_to_die = (&monitor->args->info[0])->time_to_die;
	const int time_to_die = monitor->info->time_to_die;
	const int num_of_philo = monitor->info->num_of_philo;

	while (1)
	{
		i = -1;
		time = get_time();
		while (++i < num_of_philo)
		{
			if (time - (&monitor->args[i])->last_meal_time >= time_to_die)
				return ((void*)0);
		}
	}
}

pthread_t	*init_philo(int num_of_philo)
{
	pthread_t	*philos;

	philos = malloc(sizeof(pthread_t) * num_of_philo);
	return (philos);
}

pthread_mutex_t	*init_fork(int num_of_philo)
{
	pthread_mutex_t *forks;
	int				i;

	 forks = malloc(sizeof(pthread_mutex_t) * num_of_philo);
	 if (!forks)
		 return (NULL);
	i = -1;
	while (++i < num_of_philo)
		pthread_mutex_init(&forks[i], NULL);
	return (forks);
}

t_arg *init_arg(t_arg *arg, t_info *info, int p_id, long start)
{
	arg->info = info;
	arg->p_id = p_id;
	arg->last_meal_time = start;
	return (arg);
}

long    get_time(void)
{
	struct timeval  tp;
	long            milliseconds;

	gettimeofday(&tp, NULL);
	milliseconds = tp.tv_sec * 1000;
	milliseconds += tp.tv_usec / 1000;
	return (milliseconds);
}

void grab_fork(int p_id, int fork_id, pthread_mutex_t *fork)
{
	pthread_mutex_lock(&fork[fork_id - 1]);
	printf("%ld %d has taken a fork %d\n", get_time(), p_id, fork_id);
}

void drop_forks(int p_id, pthread_mutex_t *fork, int num_of_philo)
{
	if (p_id == 1)
	{
		pthread_mutex_unlock(&fork[num_of_philo - 1]);
		pthread_mutex_unlock(&fork[0]);
	}
	else
	{
		pthread_mutex_unlock(&fork[p_id - 2]);
		pthread_mutex_unlock(&fork[p_id - 1]);
	}
}
