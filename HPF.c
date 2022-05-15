
struct ProcessStruct *currentRunningProcess;
bool isRunning;

void runProcess(struct ProcessStruct *d) {
    isRunning = true;
    currentRunningProcess = d;
    int pid = fork();
    if (pid == -1) {
        perror("Error in execl");
        exit(-1);
    }
    if (pid == 0) {
        char remainingTime[10];
        sprintf(remainingTime, "%d", d->runTime);

        printf("====================================================");
        printf("\nprocess with id =  %d started with pid = %d \n", d->id, getpid());
        printf("====================================================");
        fflush(stdout);
        print_process_info(d, 0);
        d->pid = getpid();
        char *args[] = {"./process.out", remainingTime, NULL};
        int execlResult = execvp(args[0], args);

        if (execlResult == -1) {
            perror("Error in execl");
            exit(-1);
        }
    }

}


void processFinishedHandler(int signum) {
    isRunning = false;

    print_process_info(currentRunningProcess, 3);
    signal(SIGUSR2, processFinishedHandler);
}

void HPF(struct PQueue *pq) {
    struct PQueue *processesQueue;
    struct ProcessStruct *readyProcess;
    signal(SIGUSR2, processFinishedHandler);
    isRunning = false;
    printf("HPF Started\n");
    processesQueue = pq;
    while (flag) {
        while (isEmpty(processesQueue)) {

        }
        if (!isRunning) {
            readyProcess = peek(processesQueue);
            pop(processesQueue);

            runProcess(readyProcess);
        }
    }
}