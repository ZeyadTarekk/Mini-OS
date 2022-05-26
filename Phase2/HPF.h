
struct ProcessStruct *currentRunningProcess;
bool isRunning;

void runProcess(struct ProcessStruct *d) {
    isRunning = true;
    currentRunningProcess = d;
    currentRunningProcess->waitingTime = getClk() - currentRunningProcess->arrivalTime;
    int pid = fork();
    if (pid == -1) {
        perror("Error in execl");
        exit(-1);
    }
    if (pid == 0) {
        int lengthOfRemainingTime = snprintf(NULL, 0, "%d", d->runTime);
        char *remainingTime = malloc(lengthOfRemainingTime + 1);
        snprintf(remainingTime, lengthOfRemainingTime + 1, "%d", d->runTime);

        printf("====================================================");
        printf("\nprocess with id =  %d started with pid = %d current time = %d \n", d->id, getpid(), getClk());
        printf("====================================================\n");
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
    currentRunningProcess->executionTime = currentRunningProcess->runTime;
    print_process_info(currentRunningProcess, 3);

//    send the process to be deallocated
    //send the process to memory process using the message queue
    message.mtype = 9;
    message.process = *currentRunningProcess;
    int send_val = msgsnd(msgq_id, &message, sizeof(message.process), !IPC_NOWAIT);

    printf("message sent from HPF: %d\n", message.process.id);
    if (send_val == -1)
        perror("Errror in send");
    kill(memorypid, SIGUSR2);
    free(currentRunningProcess);
    signal(SIGUSR2, processFinishedHandler);
}

void HPF(struct PQueue *pq) {
    struct PQueue *processesQueue;
    struct ProcessStruct *readyProcess;
    signal(SIGUSR2, processFinishedHandler);
    isRunning = false;
    printf("HPF Started\n");
    processesQueue = pq;
    while (flag || !isEmpty(processesQueue) || isRunning) {
        if (isEmpty(processesQueue))
            continue;
        if (!isRunning) {
            readyProcess = peek(processesQueue);
            pop(processesQueue);

            runProcess(readyProcess);
        }
    }
}