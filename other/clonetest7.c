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

    printf(1, "[clonetest7: clone and join stress test]\n");    
    
    printf(1, "1. Max processes is 64. 3 threads already in use: init, sh and this parent thread\
        \n   We should only create 61 threads\n");
    // init, sh = 2 threads
    // parent thread = 1 thread
    // remaining threads = 63
    // 1) Allocate PGSIZE + PGSIZE bytes so we can align a 4 KB page for the thread stack
    buf = malloc(PGSIZE + 64 * PGSIZE);
    if (buf == 0) {
        printf(1, "clonetest7: malloc failed\n");
        exit();
    }
    // Round up to the next 4 KB boundary
    stack = (void*)(((uint)buf + PGSIZE) & ~(PGSIZE - 1));

    for(int i = 0; i < 64; i++){
        tid = clone(stack + PGSIZE * i);
        if (tid < 0) {
            printf(1, "Parent: Failed to create child thread no. %d\n", i+1);
        }
        if (tid == 0) {
            // In the child thread
            sleep(5 + 5 * i);
            printf(1, "Child thread no. %d\n", i+1);
            exit();  // terminate this thread
        }
    }

    // 4) Wait for the child thread to exit
    for(int i = 0; i < 64; i++){
        jtid = join();
        if (jtid < 0) {
            // printf(1, "Parent: Failed to join thread\n");
        } else {
            // printf(1, "Parent: Successfully joined thread tid = %d\n", jtid);
        }
    }

    // 5) Free the buffer used for the thread’s stack
    free(buf);

    exit();
}
