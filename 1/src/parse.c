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




void get_command(int i){
    
    // cat < test.txt | grep -n public > test2.txt &
    int j = 0;
    
    while(*lineptr != '\0'){
        while(*lineptr == ' ' || *lineptr == '\t'){
               *lineptr++;
        }

        cmd[i].args[j]=avptr;
        while(*lineptr != '\0' 
		&& *lineptr != ' '
		&& *lineptr != '\t'
		&& *lineptr != '>'
		&& *lineptr != '<'
		&& *lineptr != '|'
		&& *lineptr != '&'
		&& *lineptr != '\n'){
		*avptr++ = *lineptr++;

        }

        *avptr++ = '\0';

        switch(*lineptr){
	case ' ':
	case '\t':
		j++;
		break;
        case '<':
	case '>':
	case '|':
//	case '>':
	case '\n':
		return;
	default:
		return;
	}


    }    
}


void getname(char *name){

}

int check(const char *str){


}




int parse_command(void){
    // cat < test.txt | grep -n public > test2.txt &
    // 1
    get_command(0);
    // 2
    if(check("<")){
        getname(infile);
    }
    // 3
    int i;
    for (i=1; i<PIPELINE; ++i){
        if(check("|")){
            get_command(i);
        }
        else{
	    break;
        }
    }

    // 4
    if(check(">")){
        getname(outfile);

    }

    // 5
    if(check("&")){

        backgnd = 1;
    }
    // 6
    if(check("\n")){
        cmd_count = i;
        return cmd_count;
    }
    else{
         fprintf(stderr, "Command line syntax error\n");

        return -1;
    }

    return 0;

}

int execute_command(void){
/*    
    pid_t pid = fork();

    if(-1==pid){
         perror("fork");
        exit(EXIT_FAILURE);

    }

    if(0==pid){

        execvp(cmd.args[0], cmd.args);
    }

    wait(NULL);
*/  
  return 0;
}
