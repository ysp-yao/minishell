#include "parse.h"
#include "init.h"
#include "def.h"

char cmdline[MAXLINE+1];
char avline[MAXLINE+1];

char infile[MAXNAME+1];
char outfile[MAXNAME+1];

int cmd_count;
int backgnd;


COMMAND cmd[PIPELINE];



char *lineptr;
char *avptr;



int main(){


    setup();

    shell_loop();

    return 0;
}
