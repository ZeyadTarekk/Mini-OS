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
* Release all unreleased resources resource (ipcrm -q [id])

### Testing

* Send signal 5 (SIGTRAP) for the scheduler to make it print the **priority** queue at any time

### Needed to be handled
* Zero running time process