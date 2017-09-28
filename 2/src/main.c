#include "parse.h"
#include "init.h"
#include "def.h"

char cmdline[MAXLINE+1];
COMMAND cmd;
char avline[MAXLINE+1];
int main(){

    setup();

    shell_loop();

    return 0;
}
