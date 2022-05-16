# OS-Project

## General notes

### In scheduler

* There is a variable called flag this flag is used to terminate the scheduler
* This flag is one until a process of a given id is sent to the scheduler (currently it's -1)
* So make the last process id -1 (this process is ignored : dummy process) or specify your id and modify process.txt
  file
* note that this dummy process has to have an arrival time greater than the sum of all running time of all process in
  addition to any delays (make its arrival time large enough to make sure that all processes has finished execution)

### One each run

* Make sure that all process have been terminated use (ps -a)
* Kill all alive processes (kill -9 [pid])
* Make sure that all resources have been released use (ipcs)
* Release all unreleased resources resource 
  * (ipcrm -q [id]) for message queue
  * (ipcrm -s [id]) for semaphore
  * (ipcrm -m [id]) for shared memory

### Testing

* Send signal 5 (SIGTRAP) for the scheduler to make it print the **priority** queue at any time

### Needed to be handled
* [x] Zero running time process
* [ ] Exit flag (Shared memory) **process generator** and **Scheduler**
* [ ] Total processes in the input file to calculate the WTA in the scheduler.log **process generator**
* [ ] Assign the enterQueueTime to the arrival time in the **process generator**
* [ ] A function to create **_scheduler.perf_** which contains summary statistics

### Note from project document
* If your algorithm does a lot of processing, processes might not start and stop
  at the same time instance. Then, your utilization should be less than 100%.