#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"

void thread_main(void* arg)
{
    printf(1, "Hello world! I shouldn't be cut off by the other guy. That would be rude of that person. Just let me talk, okay?\n");

	return;
}

int main(int argc, char** argv)
{
	// int id = 10;
	int tid1, tid2;
	printf(1, "[bonustest: Test multi-thread printf]\n");

	tid1 = thread_create(thread_main, &tid1);
	tid2 = thread_create(thread_main, &tid2);

	thread_join(tid1);
	thread_join(tid2);

	printf(1, "Test done. Both messages should not be cut off.\n");

	exit();
}

