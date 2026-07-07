#include <stdio.h>
#include <pthread.h>

void *worker(void *arg)
{
	printf("Hello from the thread!\n");
	return NULL;
}

int main()
{
	pthread_t thread;

	pthread_create(&thread, NULL, worker, NULL);

	pthread_join(thread, NULL);

	printf("Main thread finished.\n");

	return 0;
}