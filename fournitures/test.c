#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h> /* wait */
#include "readcmd.h"


static void handler(int signum)
{
    printf("ress");
    //exit(EXIT_SUCCESS);
}


int main()
{
    struct sigaction sa;


    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART; /* Restart functions if
                                 interrupted by handler */
    if (sigaction(SIGTSTP, &sa, NULL) == -1)
        /* Handle error */;


    while(1) {

    }
}