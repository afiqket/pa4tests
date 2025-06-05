#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"

void thread_main(void* arg)
{
	int* argint = arg;
	sleep(*argint * 10); 
	printf(1, "%d. Hello! from child no. %d\n", *argint, *argint - 1);
	return;
}

int main(int argc, char** argv)
{
	int t1 = 2, t2 = 3, t3 = 4;
	int tid1, tid2, tid3;
	printf(1, "test3...\n");
    
	tid1 = thread_create(thread_main, &t1);
	tid2 = thread_create(thread_main, &t2);
	tid3 = thread_create(thread_main, &t3);
    
    printf(1, "1. Created 3 threads\n");

	thread_join(tid1);
	thread_join(tid2);
	thread_join(tid3);

	printf(1, "5. Test done. Prints should be in order 1-5\n");

	exit();
}

