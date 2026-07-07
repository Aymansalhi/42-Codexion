#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *worker(void *arg)
{
	printf("Hello from the thread! 0 \n");
	return NULL;
}

void *worker1(void *arg)
{
	printf("Hello from the thread! 1 \n");
	return NULL;
}

int main()
{
	pthread_t thread;

	pthread_create(&thread, NULL, worker, NULL);

	// pthread_join(thread, NULL);

	pthread_t thread1;

	pthread_create(&thread1, NULL, worker1, NULL);

	// pthread_join(thread1, NULL);

	printf("sleeping\n");
	sleep(10);
	printf("Main thread finished.\n");

	return 0;
}