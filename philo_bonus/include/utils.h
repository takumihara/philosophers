#ifndef UTILS_H
# define UTILS_H

bool	atoi_strict(char *str, int *dst);
long	get_time(void);
int		max(int a, int b);
void	sleep_precisely(long start, long wait_time);

#endif //UTILS_H
