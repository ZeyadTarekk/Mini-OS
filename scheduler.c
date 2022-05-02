#include "headers.h"


int main(int argc, char * argv[])
{
    initClk();
    //get the algorithm number
    int algorithm = atoi(argv[1]);
    printf("The algorithm chosen is: %d\n", algorithm);
    //TODO implement the scheduler :)
    //upon termination release the clock resources.
    
    destroyClk(true);
}
