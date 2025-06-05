// ERROR.c
#include "types.h"
#include "stat.h"
#include "user.h"

#define PGSIZE 4096

int
main(int argc, char *argv[])
{
    char *buf = 0;
    void *stack;
    int tid, jtid, waitret, childpid;
    printf(1, "test2...\n");

    // ---- Allocate PGSIZE + PGSIZE bytes so we can align a 4 KB page for the thread stack ---- 
    buf = malloc(PGSIZE + PGSIZE);
    if (buf == 0) {
        printf(1, "ERROR: malloc failed\n");
        exit();
    }
    // Round up to the next 4 KB boundary
    stack = (void*)(((uint)buf + PGSIZE) & ~(PGSIZE - 1));

    // ---- Create a thread ---- 
    tid = clone(stack);
    if (tid < 0) {
        printf(1, "ERROR: clone failed\n");
        free(buf);
        exit();
    }
    if (tid == 0) {
        // In the child thread
        sleep(20);
        printf(1, "4. Child thread: Finish second\n");
        exit();  // terminate this thread
    }

    // ---- Create a child process ---- 
    childpid = fork();
    if (childpid < 0) {
        printf(1, "ERROR: fork failed\n");
        free(buf);
        exit();
    }
    if (childpid == 0) {
        // In the child thread
        sleep(10);
        printf(1, "3. Child process: Finish first\n");
        exit();  // terminate this thread
    }

    printf(1, "1. Parent: created a thread and child process\n");
    
    printf(1, "2. Parent: wait for thread first\n");
    jtid = join();
    // printf(1, "Parent: jtid = %d\n", jtid);
    if (jtid < 0) {
        printf(1, "ERROR: join failed\n");
        exit();
    } else {
        printf(1, "5. Parent: joined thread\n");
    }

    waitret = wait();
    // printf(1, "Parent: waitret = %d\n", waitret);
    if (waitret < 0) {
        printf(1, "ERROR: wait failed\n");
        exit();
    } else {
        printf(1, "6. Parent: done wait for child process\n");
    }

    printf(1, "7. Test done. Prints should be in order 1-7\n");

    // 5) Free the buffer used for the thread’s stack
    free(buf);
    exit();
}
