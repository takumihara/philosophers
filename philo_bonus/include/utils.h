#ifndef UTILS_H
# define UTILS_H

bool		atoi_strict(char *str, int *dst);
long long	get_usec(void);
int			max(int a, int b);
void		msleep_precise(long long start, long long wait_time);

#endif //UTILS_H
