#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

#define PGSIZE 4096
#define BUFFERSIZE 360

int
main(int argc, char *argv[])
{
    char *buf = 0;
    void *stack1, *stack2;
    int *fd_pointer;
    char readbuffer[BUFFERSIZE];
    int tid1, tid2, jtid1, jtid2, readret;

    printf(1, "[clonetest8: Test file open/read. A thread should read a file opened by another thread.]\n");

    // 1) Allocate PGSIZE + PGSIZE bytes so we can align a 4 KB page for the thread stack
    buf = malloc(PGSIZE + PGSIZE + PGSIZE);
    if (buf == 0) {
        printf(1, "clonetest4: malloc failed\n");
        exit();
    }
    // Round up to the next 4 KB boundary
    stack1 = (void*)(((uint)buf + PGSIZE) & ~(PGSIZE - 1));
    stack2 = (void*)((uint)stack1 + PGSIZE);
    // printf(1, "stack1=0x%x\n", (uint)stack1);
    // printf(1, "stack2=0x%x\n", (uint)stack2);

    fd_pointer = malloc(sizeof(int));

    // 2) Create a single thread
    tid1 = clone(stack1);
    if (tid1 < 0) {
        printf(1, "Parent: clone failed\n");
        free(buf);
        exit();
    }
    if (tid1 == 0) {
        sleep(10);
        printf(1, "Thread 1: Opening file...\n");
        *fd_pointer = open("README", O_RDONLY);
        if (*fd_pointer < 0){
            printf(1, "Thread 1: ERROR: Open file fail!\n");
        }
        printf(1, "Thread 1: Open file success! fd = %d\n", *fd_pointer);
        exit(); 
    }


    tid2 = clone(stack2);
    if (tid2 < 0) {
        printf(1, "Parent: clone failed\n");
        free(buf);
        exit();
    }
    if (tid2 == 0) {
        sleep(50);
        printf(1, "Thread 2: Reading file fd = %d\n", *fd_pointer);
        readret = read(*fd_pointer, readbuffer, sizeof(readbuffer));
        if (readret < 0){
            printf(1, "Thread 2: ERROR: Read file fail!\n");
        } else {
            readbuffer[BUFFERSIZE - 1] = 0;
            printf(1, "Thread 2: Read success! Printing contents...\n\n");
            printf(1, readbuffer);
            printf(1, "\n\n");
        }
        exit(); 
    }

    // 3) Back in the parent: clone() returned child’s TID
    printf(1, "Parent: Created threads\n");

    // 4) Wait for the child thread to exit
    jtid1 = join();
    if (jtid1 < 0) {
        printf(1, "clonetest4: join failed\n");
    } else {
        printf(1, "Parent: Joined thread (tid = %d)\n", jtid1);
    }

    jtid2 = join();
    if (jtid2 < 0) {
        printf(1, "clonetest4: join failed\n");
    } else {
        printf(1, "Parent: Joined thread (tid = %d)\n", jtid2);
    }

    printf(1, "Test done. There should not be any ERRORs\n", jtid2);


    
    // 5) Free the buffer used for the thread’s stack
    close(*fd_pointer);
    free(buf);
    free(fd_pointer);

    exit();
}
