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

    printf(1, "1. testclone...\n");

    // 1) Allocate PGSIZE + PGSIZE bytes so we can align a 4 KB page for the thread stack
    buf = malloc(PGSIZE + PGSIZE + PGSIZE);
    if (buf == 0) {
        printf(1, "ERROR: malloc failed\n");
        exit();
    }
    // Round up to the next 4 KB boundary
    stack1 = (void*)(((uint)buf + PGSIZE) & ~(PGSIZE - 1));
    stack2 = (void*)((uint)stack1 + PGSIZE);
    // printf(1, "stack1=0x%x\n", (uint)stack1);
    // printf(1, "stack2=0x%x\n", (uint)stack2);

    // 2) Create a single thread
    tid1 = clone(stack1);
    if (tid1 < 0) {
        printf(1, "ERROR: Parent: clone failed\n");
        free(buf);
        exit();
    }
    if (tid1 == 0) {
        // In the child thread B
        tid2 = clone(stack2);
        // printf(1, "Thread B or C: tid = %d\n", tid2);
        if (tid2 < 0) {
            sleep(50);
            printf(1, "ERROR: Thread B: clone failed\n");
            free(buf);
            exit();
        }
        if (tid2 == 0) {
            // In the child thread C
            sleep(20);
            printf(1, "5. Thread C: Hello world!\n");
            exit(); 
        }
        sleep(10);
        printf(1, "3. Thread B: Created thread C (tid = %d, pid = %d)\n", tid2, getpid());
        exit(); 
    }

    // 3) Back in the parent: clone() returned child’s TID
    sleep(10);
    printf(1, "2. Parent: Created thread B (tid = %d, pid = %d)\n", tid1, getpid());

    // 4) Wait for the child thread to exit
    jtid1 = join();
    if (jtid1 < 0) {
        printf(1, "ERROR: first join failed\n");
    } else {
        printf(1, "4. Parent: successfully joined thread B (tid = %d)\n", jtid1);
    }

    jtid2 = join();
    if (jtid2 < 0) {
        printf(1, "ERROR: second join failed\n");
    } else {
        printf(1, "6. Parent: successfully joined thread C (tid = %d)\n", jtid2);
    }

    printf(1, "7. Test done. There should not be any zombies or crashes\n", jtid2);


    // 5) Free the buffer used for the thread’s stack
    free(buf);

    exit();
}
