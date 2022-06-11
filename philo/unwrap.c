#include <assert.h>
#include <pthread.h>

void	pthread_mutex_lock_unwrap(pthread_mutex_t *mutex)
{
	assert(pthread_mutex_lock(mutex) == 0);
}

void	pthread_mutex_unlock_unwrap(pthread_mutex_t *mutex)
{
	assert(pthread_mutex_unlock(mutex) == 0);
}
