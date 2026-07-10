#include <stdio.h>
#include <sys/time.h>

int main(void)
{
    struct timeval tv;

    if (gettimeofday(&tv, NULL) == -1)
        return (1);

    printf("Seconds      : %ld\n", tv.tv_sec);
    printf("Microseconds : %ld\n", tv.tv_usec);
}