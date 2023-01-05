*************************************
OS : A3 : Q1 PART-B : Semaphores
*************************************

This is a program to simulate the philosophers dining problem with 2 shared suace bowls using semaphores.

I have created an array of sems for forks, and another for the sems of bowls.
Each time when a philosopher acquires lock for both the forks, it checks which bowl is free. If any one of them is free, it takes it, else waits till first bowl gets free.
After it eats, it posts all the acquired semaphores.

Steps to run;
    1. Just run the Makefile (make sure Makefile is present in the folder just outside src and bin)

Thank You!