// clonetest2.c
#include "types.h"
#include "stat.h"
#include "user.h"

#define PGSIZE 4096

int
main(int argc, char *argv[])
{
    char *buf = 0;
    void *stack;
    int tid, jtid;

    printf(1, "[clonetest2: Test correctness of clone and join (heap sharing)]\n");

    // 1) Allocate PGSIZE + PGSIZE bytes so we can align a 4 KB page for the thread stack
    buf = malloc(PGSIZE + PGSIZE);
    if (buf == 0) {
        printf(1, "clonetest2: malloc failed\n");
        exit();
    }
    // Round up to the next 4 KB boundary
    stack = (void*)(((uint)buf + PGSIZE) & ~(PGSIZE - 1));

    int *numbers = malloc(10 * sizeof(int));
    for (int i = 0; i < 10; i++) {numbers[i] = i;}

    // 2) Create a single thread
    tid = clone(stack);
    if (tid < 0) {
        printf(1, "clonetest2: clone failed\n");
        free(buf);
        exit();
    }
    if (tid == 0) {
        // In the child thread
        sleep(50);
        printf(1, "Test heap should be shared: Should print 10, not 0: %d\n", numbers[0]);
        printf(1, "Child thread: clone() returned 0, pid = %d\n", getpid());
        exit();  // terminate this thread
    }

    // 3) Back in the parent: clone() returned child’s TID
    printf(1, "Parent: created one thread (tid = %d), pid = %d\n", tid, getpid());

    numbers[0] = 10;
    printf(1, "Test heap should be shared: Should print 10, not 0: %d\n", numbers[0]);


    // 4) Wait for the child thread to exit
    jtid = join();
    if (jtid < 0) {
        printf(1, "clonetest2: join failed\n");
    } else {
        printf(1, "Parent: successfully joined thread tid = %d\n", jtid);
    }

    // 5) Free the buffer used for the thread’s stack
    free(buf);

    exit();
}
