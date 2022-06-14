#ifndef UTILS_H
# define UTILS_H

bool		atoi_strict(char *str, int *dst);
long long	get_usec(void);
long long	max(long long a, long long b);
void		usleep_until(long long time);
void		ft_putstr_fd(const char *s, int fd);

#endif //UTILS_H
