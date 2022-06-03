#include <limits.h>
#include <sys/time.h>

#include <stdbool.h>
#include <unistd.h>

bool	atoi_strict(char *str, int *dst)
{
	long long	rtn;
	int			sign;

	rtn = 0;
	sign = 1;
	while (('\t' <= *str && *str <= '\r') || *str == ' ')
		str++;
	if (*str == '-')
		sign = -1;
	if (*str == '+' || *str == '-')
		str++;
	if (*str == '\0')
		return (false);
	while ('0' <= *str && *str <= '9')
	{
		if ((rtn * 10 + *str - '0') / 10 != rtn)
			return (false);
		rtn = rtn * 10 + *str - '0';
		str++;
	}
	*dst = (int)(sign * rtn);
	if (*str == '\0')
		return (true);
	return (false);
}

long long	get_usec(void)
{
	struct timeval	tp;
	long long		usec;

	gettimeofday(&tp, NULL);
	usec = tp.tv_sec * 1000 * 1000;
	usec += tp.tv_usec;
	return (usec);
}

int	max(int a, int b)
{
	if (a >= b)
		return (a);
	else
		return (b);
}

void	usleep_precise(long long start, long long wait_time)
{
	while (1)
	{
		if (get_usec() >= start + wait_time)
			return ;
		usleep(500);
	}
}
