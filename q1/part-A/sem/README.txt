*************************************
OS : A3 : Q1 PART-A : Semaphores
*************************************

This is a program to simulate the philosophers dining problem using semaphores.

I have created an array of sems for forks.
Each time ith philosopher wants to eat, it checks is ith and (i+1)th forks are free. If both of them are free, it starts eating, else waits.
After it eats, it posts all the acquired semaphores.

Steps to run;
    1. Just run the Makefile (make sure Makefile is present in the folder just outside src and bin)

Thank You!
