#ifndef UTILS_H
# define UTILS_H

bool		atoi_strict(char *str, int *dst);
long long	get_usec(void);
int			max(int a, int b);
void		usleep_precise(long long start, long long wait_time);
void		ft_putstr_fd(const char *s, int fd);

#endif //UTILS_H
