#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h> /* wait */

static volatile int keepRunning = 1;
void intHandler(int dummy) {
    printf("ctrl-z");
    keepRunning = 0;
}


int main() {   
    signal(SIGINT, intHandler);
    while (keepRunning) { 

        }
}