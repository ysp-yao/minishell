#include "parse.h"
#include "init.h"
#include "def.h"

char cmdline[MAXLINE+1];
char avline[MAXLINE+1];
COMMAND cmd;

int main(void)
{
	setup();
	shell_loop();
	return 0;
}
