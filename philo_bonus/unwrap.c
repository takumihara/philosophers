#include <semaphore.h>
#include <assert.h>

void	sem_wait_unwrap(sem_t *sem)
{
	assert(sem_wait(sem) == 0);
}

void	sem_post_unwrap(sem_t *sem)
{
	assert(sem_post(sem) == 0);
}
