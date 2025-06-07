#include "types.h"
#include "stat.h"
#include "user.h"

#define PGSIZE 4096

int
main(int argc, char *argv[])
{
    char *buf = 0;
    void *stack, *unaligned_stack;
    int tid, jtid, forkret;
    // int padding[950] = {0};
    // printf(1, "addr of padding[0]    = 0x%x\n", &padding[0]);
    // printf(1, "addr of padding[950] = 0x%x\n", &padding[950]);

    printf(1, "testerror...\n");

    jtid = join();
    printf(1, "1. Join with no threads (should fail and return -1)\n");
    if (jtid == -1){
        printf(1, "\tCORRECT: join() = %d\n", jtid);        
    } else {
        printf(1, "\tWRONG: join() = %d\n", jtid);  
    }


    // 1) Allocate PGSIZE + PGSIZE bytes so we can align a 4 KB page for the thread stack
    buf = malloc(PGSIZE + PGSIZE);
    if (buf == 0) {
        printf(1, "ERROR: malloc failed\n");
        exit();
    }
    // Round up to the next 4 KB boundary
    stack = (void*)(((uint)buf + PGSIZE) & ~(PGSIZE - 1));

    unaligned_stack = (void*)((uint)stack + 1);

    // 2) Create a single thread
    printf(1, "2. Clone with stack that is not page-aligned (should fail) (stack = 0x%x)\n", unaligned_stack);
    tid = clone(unaligned_stack);
    if (tid == -1) {
        printf(1, "\tCORRECT: clone() failed and returned %d\n", tid);
    }
    else {
        // In the child thread
        sleep(10);
        printf(1, "\tWRONG: clone() succeeded even with unaligned stack\n");
        exit();  // terminate this thread
    }

    tid = clone(stack);
    if (tid < 0) {
        printf(1, "\tERROR: clone failed\n");
        free(buf);
        exit();
    }
    if (tid == 0) {
        // In the child thread
        sleep(10);
        forkret = fork();
        if (forkret >= 0){
            exit();
        }
        printf(1, "3. Thread child invokes fork() (should fail and return -1)\n");
        if (forkret == -1){
            printf(1, "\tCORRECT: forkret() = %d\n", forkret);        
        } else {
            printf(1, "\tWRONG: forkret() = %d\n", forkret);  
        }

        jtid = join();
        printf(1, "4. Thread child invokes join() (should fail and return -1)\n");
        if (jtid == -1){
            printf(1, "\tCORRECT: join() = %d\n", jtid);        
        } else {
            printf(1, "\tWRONG: join() = %d\n", jtid);  
        }

        
        exit();  // terminate this thread
    }

    // 3) Back in the parent: clone() returned child’s TID
    // printf(1, "(Parent: created thread)\n");

    // 4) Wait for the child thread to exit
    jtid = join();
    if (jtid < 0) {
        printf(1, "ERROR: join failed\n");
    } 
    // else {
    //     printf(1, "(Parent: joined thread)\n");
    // }

    jtid = join();
    printf(1, "5. All threads should be joined by now. No threads left. Join should fail\n");
    if (jtid == -1){
        printf(1, "\tCORRECT: join() = %d\n", jtid);        
    } else {
        printf(1, "\tWRONG: join() = %d\n", jtid);  
    }

    printf(1, "6. Test done. All tests from 1-5 must print CORRECT.\n");

    // 5) Free the buffer used for the thread’s stack
    free(buf);

    exit();
}
