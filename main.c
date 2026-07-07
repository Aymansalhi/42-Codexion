#include <stdio.h>
#include <pthread.h>

void *worker(void *arg)
{
	printf("Worker\n");
	return NULL;
}

int main()
{
	pthread_t t;

	pthread_create(&t, NULL, worker, NULL);

	printf("Main\n");

	return 0;
}