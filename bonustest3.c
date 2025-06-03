#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"

#define ITERS 100 // Number of iteration of loop
#define BYTES 100 // Number of bytes to request from sbrk

void thread_main(void* arg)
{
    sleep(20);
    printf(1, "Initial size = %d\n", sbrk(0));
    sleep(20);

    for (int i = 0; i < ITERS; i++){
        sbrk(BYTES);
        // printf(1, "iter %d, current size = %d\n", i, sbrk(0));
    }

	return;
}

int main(int argc, char** argv)
{
	// int id = 10;
	int tid1, tid2;
	printf(1, "[bonustest3: Test multi-thread sbrk()]\n");

	tid1 = thread_create(thread_main, &tid1);
	tid2 = thread_create(thread_main, &tid2);

	thread_join(tid1);
	thread_join(tid2);

	printf(1, "Test done. There should be no crashes. \nCurrent size = %d. This should be equal to initial size + %d.\n", sbrk(0), ITERS*BYTES);

	exit();
}

