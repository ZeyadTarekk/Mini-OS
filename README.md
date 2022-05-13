# OS-Project

## General notes
### In scheduler
* There is a variable called flag this flag is used to terminate the scheduler
* This flag is zero until a process of a given id is sent to the scheduler (currently it's 11)
* So make the last process id 11 (this process is ignored : dummy process) or specify your id

### One each run
  * Make sure that all process have been terminated use (ps -a)
  * Kill all alive processes (kill -9 [pid])
  * Make sure that all resources have been released use (ipcs) 
  * Release all unreleased resources resource (ipcrm -q [id])