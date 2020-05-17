#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h> /* wait */
#include "readcmd.h"



int main()
{   
    
    struct cmdline *cmd;
    cmd = readcmd();
    printf("%s", cmd->seq[0][0]);

}