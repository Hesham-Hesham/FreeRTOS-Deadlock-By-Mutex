#  FreeRTOS Deadlock by Mutex
 - In an operating system, a deadlock occurs when a process or thread enters a waiting state because a requested system resource is held by another waiting process, which in turn is waiting for another resource held by another waiting process.

## Implementation


 - In this example it is implemented by using two different Mutexes that are being used by two tasks.
 - The first task T1 takes mutex1 then requests mutex2 and gets blocked then the second task T2 takes mutex2 then requests mutex1 which is already taken by the blocked T1. Now both tasks are in blocked state because of eachother and they will never be able to run again!

### Solution

- Deadlock could be solved by keeping the same order in acquiring and releasing the resources
- It could also be solved when the OS forces the resource to be released from the task holding it







# Software Used

 1. Keil uVision5 
 2. PuTTY to simulate the UART communication with the microcontroller
    

## Explaination Video

> **You can watch the e xplaination video in arabic** [here](https://drive.google.com/file/d/1xc1sXiBg9BBzJa97DDbBG0EnJzIp6HtT/view?usp=sharing).
##








$$
  \int_0^\infty\infty  dt 
$$
