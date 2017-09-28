#include "init.h"
#include "extern.h"
#include <stdio.h>
#include <signal.h>
#include <string.h>

void sigint_handler(int sig){

    printf("\n[minishell]$ ");
    fflush(stdout);

}

void init(void){

    memset(&cmd, 0, sizeof(cmd));
    memset(cmdline, 0, sizeof(cmdline));

}

void setup(void){

    signal(SIGINT, sigint_handler);
    signal(SIGQUIT, SIG_IGN);


}
