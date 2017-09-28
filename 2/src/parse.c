#include "parse.h"
#include "init.h"
#include <stdio.h>
#include "def.h"
#include "extern.h"

#include <unistd.h> 
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

void shell_loop(void){
    while(1){
        printf("[minishell]$ ");
        fflush(stdout);
        init();
        if(-1 == read_command()){
            break;
        }
        parse_command();
        execute_command();
    }
    printf("\nexit!\n");
}
int read_command(void){
    if(NULL == fgets(cmdline, MAXLINE, stdin)){
	return -1;
    }

    return 0;
}
int parse_command(void){
    char *cp = cmdline;
    char *avp = avline;
    int i = 0;

    while('\0' != *cp){
        while(' ' == *cp || '\t' == *cp){
             cp++;
        }
        if('\0' == *cp || '\n' == *cp){
             break;
        }	


        cmd.args[i]=avp;

        while('\0' != *cp
		&& ' ' != *cp
		&& '\t'!= *cp
		&& '\n'!= *cp){
	    *avp++ = *cp++;
	}
        
        *avp++ = '\0';
  //      printf("[%s]\n", cmd.args[i]);
        i++;
    }    
    return 0;
}

int execute_command(void){
    
    pid_t pid = fork();

    if(-1==pid){
         perror("fork");
        exit(EXIT_FAILURE);

    }

    if(0==pid){

        execvp(cmd.args[0], cmd.args);
    }

    wait(NULL);
    return 0;
}
