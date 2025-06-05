#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"

#define NPOINTERS 200

void thread_main(void* arg)
{
    int** pointers = (int**)arg;

    for(int i = 0; i < NPOINTERS; i++){
        pointers[i] = (int*)malloc(sizeof(int));
        // printf(1, "pointers[%d] = %d\n", i, pointers[i]);
        if (pointers[i] == 0)
            printf(1, "malloc fail for pointer[%d]\n", i);
        // else
        //     printf(1, "malloc success for pointer[%d]\n", i);
    }

    sleep(10);

    for(int i = 0; i < 100; i++){
        free(pointers[i]);
    }

	return;
}

int main(int argc, char** argv)
{
	// int id = 10;
	int tid1, tid2;
    int **pointers1, **pointers2;
	printf(1, "[bonustest2: Test multi-thread malloc and free]\n");

    pointers1 = malloc(2 * NPOINTERS * sizeof(int));
    pointers2 = pointers1 + NPOINTERS;

	tid1 = thread_create(thread_main, pointers1);
	tid2 = thread_create(thread_main, pointers2);

	thread_join(tid1);
	thread_join(tid2);

    for (int i = 0; i < NPOINTERS; i++) {
        for (int j = 0; j < NPOINTERS; j++) {
            if (pointers1[i] == pointers2[j]) {
                printf(1, "ERROR: Pointers not unique!");
            }
        }
    }

	printf(1, "Test done. There should be no fails and no errors\n");

	exit();
}

