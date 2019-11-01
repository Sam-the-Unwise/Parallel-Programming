# Parallel Programming with Pthreads

Problem 1;
Having multiple threads call a function, like do_work(...), will have a non-deterministic execution.
Write a program with 3 threads that call a function called do_work. Each thread will be responsible for
generating a number and appending it to a buffer. Thread 1 generates number 1, thread 2 generates number
2, and thread 3 generates number 3. These numbers assigned to the threads are passed in as arguments.
Each thread will store its value in a shared buffer of integers having a size of 3 elements called “buffer”.
When the third element is added to the buffer by either thread 1, 2 or 3, then it checks to see if the sequence
is “123”. If not, it clears the buffer and the threads try to generate the sequence again. Once the total
number of sequences of “123” reach 10, the threads should exit the function and join with the main thread.
Each time you generate “123”, it should be printed to the screen. You should also print out the total number
page 1 of 4
CS499: Introduction to Parallel Programming
of tries it took to print “123”. For example, keep track of the total number of other sequences generated
(including 123), as well: 321, 213, etc. You must use the usleep(500000) function once at each iteration.
But you can change this to test to see if your program works correctly (like removing the sleep altogether).
Also, each time a thread adds its element to the buffer, it should print out it’s corresponding number.
