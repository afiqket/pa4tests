# other

Files contained in the "other" folder: 

## clonetests
1. Test basic functionalities of clone and join. Does not test edge cases.
	- Should clone successfully
	- Stack should be copied succesfully (crashes if not copied)
	- Should join succesfully
2. Test correctness of clone and join (heap sharing)
	- Heap (malloc) should be shared by two threads. A value changed in one thread should be seen by the other thread
3. Test thread vs fork
	- join() should wait for child thread to finish, not fork child
	- wait() should wait for fork child (process child) to finish
4. Child thread can call clone, but they share the same thread parent
	- Parent thread creates a child thread, then that child thread creates another child thread.
	- Parent thread must wait for both children.
5. Test orphan thread
	- Thread exits first, but parent forgets to join() for that thread.
	- Thread must be passed to init (pid = 1). init is now its parent thread.
	- init should be waken up to properly kill the child.
6. Test simple error cases
	- Join with no threads (should fail and return -1)
	 - Clone with stack that is not page-aligned (should fail)
	 - Thread child invokes fork() (should fail and return -1)
	 - Thread child invokes join() (should fail and return -1)
	 - A second join with no threads, to check if all threads are truly joined. It should fail also since there's no threads.
7. clone and join stress test
	- Try to create 64 threads and join them.
	- It should be able to create only around 61 threads only. 
8. Test file open/read. A thread should read a file opened by another thread.

## bonustests
1. Test multi-thread printf
2. Test multi-thread malloc and free
3. Test multi-thread sbrk()
