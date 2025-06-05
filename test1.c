#include "types.h"
#include "stat.h"
#include "user.h"

#define PGSIZE 4096

int
main(int argc, char *argv[])
{
    char *buf = 0;
    void *stack1, *stack2;
    int tid1, tid2, jtid1, jtid2;

    printf(1, "test1...\n");

    // 1) Allocate PGSIZE + PGSIZE bytes so we can align a 4 KB page for the thread stack
    buf = malloc(PGSIZE * 3);
    if (buf == 0) {
        printf(1, "ERROR: malloc failed\n");
        exit();
    }
    // Round up to the next 4 KB boundary
    stack1 = (void*)(((uint)buf + PGSIZE) & ~(PGSIZE - 1));
    stack2 = (void*)((uint)stack1 + PGSIZE);

    // 2) Create a single thread
    tid1 = clone(stack1);
    if (tid1 < 0) {
        printf(1, "ERROR: first clone failed\n");
        free(buf);
        exit();
    }
    if (tid1 == 0) {
        // In the child thread
        sleep(10);
        printf(1, "3. Child thread 1: clone() returned 0, pid = %d\n", getpid());
        exit();  // terminate this thread
    }

    // 3) Back in the parent: clone() returned child’s TID
    printf(1, "1. Parent: created one thread (tid = %d), pid = %d\n", tid1, getpid());

    tid2 = clone(stack2);
    if (tid2 < 0) {
        printf(1, "ERROR: second clone failed\n");
        free(buf);
        exit();
    }
    if (tid2 == 0) {
        // In the child thread
        sleep(20);
        printf(1, "4. Child thread 2: clone() returned 0, pid = %d\n", getpid());
        exit();  // terminate this thread
    }

    printf(1, "2. Parent: created another thread (tid = %d), pid = %d\n", tid2, getpid());

    // 4) Wait for the child thread to exit
    jtid1 = join();
    if (jtid1 < 0) {
        printf(1, "ERROR: clonetest: first join failed\n");
        exit();
    } else {
        printf(1, "5. Parent: successfully joined thread tid = %d\n", jtid1);
    }

        // 4) Wait for the child thread to exit
    jtid2 = join();
    if (jtid2 < 0) {
        printf(1, "ERROR: second clonetest: join failed\n");
        exit();
    } else {
        printf(1, "6. Parent: successfully joined thread tid = %d\n", jtid2);
    }

    printf(1, "7. Test done.\n");

    // 5) Free the buffer used for the thread’s stack
    free(buf);

    exit();
}
