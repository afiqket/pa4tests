## clonetests
1. Test basic functionalities of clone and join. Does not test edge cases.
2. Test correctness of clone and join (heap sharing)
3. Test thread vs fork
4. Child thread can call clone, but they share the same thread parent
5. Test orphan thread
6. Test simple error cases
7. clone and join stress test
8. Test file open/read. A thread should read a file opened by another thread.

## bonustests
1. Test multi-thread printf
2. Test multi-thread malloc and free
3. Test multi-thread sbrk()
