// clonetest3.c
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
    printf(1, "[clonetest3: Test thread vs fork]\n");

    // ---- Allocate PGSIZE + PGSIZE bytes so we can align a 4 KB page for the thread stack ---- 
    buf = malloc(PGSIZE + PGSIZE);
    if (buf == 0) {
        printf(1, "clonetest3: malloc failed\n");
        exit();
    }
    // Round up to the next 4 KB boundary
    stack = (void*)(((uint)buf + PGSIZE) & ~(PGSIZE - 1));

    // ---- Create a thread ---- 
    tid = clone(stack);
    if (tid < 0) {
        printf(1, "clonetest3: clone failed\n");
        free(buf);
        exit();
    }
    if (tid == 0) {
        // In the child thread
        sleep(200);
        printf(1, "Child thread: Finish second\n");
        exit();  // terminate this thread
    }

    // ---- Create a child process ---- 
    childpid = fork();
    if (childpid < 0) {
        printf(1, "clonetest3: fork failed\n");
        free(buf);
        exit();
    }
    if (childpid == 0) {
        // In the child thread
        sleep(100);
        printf(1, "Child process: Finish first\n");
        exit();  // terminate this thread
    }

    printf(1, "Parent: created a thread and child process\n");
    
    printf(1, "Parent: wait for thread first\n");
    jtid = join();
    printf(1, "Parent: jtid = %d\n", jtid);
    if (jtid < 0) {
        printf(1, "clonetest3: join failed\n");
    } else {
        printf(1, "Parent: joined thread (should be printed after child thread)\n");
    }

    waitret = wait();
    printf(1, "Parent: waitret = %d\n", waitret);
    if (waitret < 0) {
        printf(1, "clonetest3: wait failed\n");
    } else {
        printf(1, "Parent: done wait for child process\n");
    }

    // 5) Free the buffer used for the thread’s stack
    free(buf);
    exit();
}
