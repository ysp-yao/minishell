#include "parse.h"
#include "externs.h"
#include "init.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <linux/limits.h>
#include <fcntl.h>

void get_command(int i);
int check(const char *str);
void getname(char *name);
void print_command();
void forkexec(int i);
/*
 * shell主循环
 */
void shell_loop(void)
{
	while (1)
	{
		printf("[minishell]$ ");
		fflush(stdout);
		init();
		if (read_command() == -1)
			break;
		parse_command();
		print_command();
		execute_command();
	}
	
	printf("\nexit\n");
}

/*
 * 读取命令
 * 成功返回0，失败或者读取到文件结束符(EOF)返回-1
 */
int read_command(void)
{
	if (fgets(cmdline, MAXLINE, stdin) == NULL)
		return -1;
	return 0;
}

/*
 * 解析命令
 * 成功返回解析到的命令个数，失败返回-1
 */
int parse_command(void)
{
	/* cat < test.txt | grep -n public > test2.txt & */
	if (check("\n"))
		return 0;

	/* 1、解析第一条简单命令 */
	get_command(0);
	/* 2、判定是否有输入重定向符 */
	if (check("<"))
		getname(infile);
	/* 3、判定是否有管道 */
	int i;
	for (i=1; i<PIPELINE; ++i)
	{
		if (check("|"))
			get_command(i);
		else
			break;
	}
	/* 4、判定是否有输出重定向符 */
	if (check(">"))
	{
		if (check(">"))
			append = 1;
		getname(outfile);
	}
	/* 5、判定是否后台作业 */
	if (check("&"))
		backgnd = 1;
	/* 6、判定命令结束‘\n’*/
	if (check("\n"))
	{
		cmd_count = i;
		return cmd_count;
	}
	else
	{
		fprintf(stderr, "Command line syntax error\n");
		return -1;
	}
}

/*
 * 执行命令
 * 成功返回0，失败返回-1
 */
int execute_command(void)
{
	/*
	pid_t pid = fork();
	if (pid == -1)
		ERR_EXIT("fork");

	if (pid == 0)
		execvp(cmd.args[0], cmd.args);

	wait(NULL);
	*/

	if (infile[0] != '\0')
		cmd[0].infd = open(infile, O_RDONLY);

	if (outfile[0] != '\0')
	{
		if (append)
			cmd[cmd_count-1].outfd = open(outfile, O_WRONLY | O_CREAT
				| O_APPEND, 0666);
		else
			cmd[cmd_count-1].outfd = open(outfile, O_WRONLY | O_CREAT
				| O_TRUNC, 0666);
	}

	/* 因为后台作不会调用wait等待子进程退出 */
	/* 为避免僵死进程，可以忽略SIGCHLD信号 */
	if (backgnd == 1)
		signal(SIGCHLD, SIG_IGN);

	int i;
	int fd;
	int fds[2];
	for (i=0; i<cmd_count; ++i)
	{
		/* 如果不是最后一条命令，则需要创建管道 */
		if (i<cmd_count-1)
		{
			pipe(fds);
			cmd[i].outfd = fds[1];
			cmd[i+1].infd = fds[0];
		}

		forkexec(i);

		if ((fd = cmd[i].infd) != 0)
			close(fd);

		if ((fd = cmd[i].outfd) != 1)
			close(fd);
	}

	if (backgnd == 0)
	{
		/* 前台作业，需要等待管道中最后一个命令退出 */
		while (wait(NULL) != lastpid)
			;
	}

	return 0;
}

void print_command()
{
	int i;
	int j;
	printf("cmd_count = %d\n", cmd_count);
	if (infile[0] != '\0')
		printf("infile=[%s]\n", infile);
	if (outfile[0] != '\0')
		printf("outfile=[%s]\n", outfile);

	for (i=0; i<cmd_count; ++i)
	{
		j = 0;
		while (cmd[i].args[j] != NULL)
		{
			printf("[%s] ", cmd[i].args[j]);
			j++;
		}
		printf("\n");
	}
}

/*
 * 解析简单命令至cmd[i]
 * 提取cmdline中的命令参数到avline数组中，
 * 并且将COMMAND结构中的args[]中的每个指针指向这些字符串
 */
void get_command(int i)
{
	/*   cat < test.txt | grep -n public > test2.txt & */

	int j = 0;
	int inword;
	while (*lineptr != '\0')
	{
		/* 去除空格 */
		while (*lineptr == ' ' || *lineptr == '\t')
			*lineptr++;

		/* 将第i条命令第j个参数指向avptr */
		cmd[i].args[j] = avptr;
		/* 提取参数 */
		while (*lineptr != '\0'
			&& *lineptr != ' '
			&& *lineptr != '\t'
			&& *lineptr != '>'
			&& *lineptr != '<'
			&& *lineptr != '|'
			&& *lineptr != '&'
			&& *lineptr != '\n')
		{
				/* 参数提取至avptr指针所向的数组avline */
				*avptr++ = *lineptr++;
				inword = 1;
		}
		*avptr++ = '\0';
		switch (*lineptr)
		{
		case ' ':
		case '\t':
			inword = 0;
			j++;
			break;
		case '<':
		case '>':
		case '|':
		case '&':
		case '\n':
			if (inword == 0)
				cmd[i].args[j] = NULL;
			return;
		default: /* for '\0' */
			return;
		}
	}
}

/*
 * 将lineptr中的字符串与str进行匹配
 * 成功返回1，lineptr移过所匹配的字符串
 * 失败返回0，lineptr保持不变
 */
int check(const char *str)
{
	char *p;
	while (*lineptr == ' ' || *lineptr == '\t')
		lineptr++;

	p = lineptr;
	while (*str != '\0' && *str == *p)
	{
		str++;
		p++;
	}

	if (*str == '\0')
	{
		lineptr = p;	/* lineptr移过所匹配的字符串 */
		return 1;
	}

	/* lineptr保持不变 */
	return 0;
}

void getname(char *name)
{
	while (*lineptr == ' ' || *lineptr == '\t')
		lineptr++;

	while (*lineptr != '\0'
			&& *lineptr != ' '
			&& *lineptr != '\t'
			&& *lineptr != '>'
			&& *lineptr != '<'
			&& *lineptr != '|'
			&& *lineptr != '&'
			&& *lineptr != '\n')
	{
			*name++ = *lineptr++;
	}
	*name = '\0';
}

void forkexec(int i)
{
	pid_t pid;
	pid = fork();
	if (pid == -1)
		ERR_EXIT("fork");

	if (pid > 0)
	{
		/* 父进程 */
		lastpid = pid;
	}
	else if (pid == 0)
	{
		/* backgnd=1时，将第一条简单命令的infd重定向至/dev/null */
		/* 当第一条命令试图从标准输入获取数据的时候立即返回EOF */

		if (cmd[i].infd == 0 && backgnd == 1)
			cmd[i].infd = open("/dev/null", O_RDONLY);

		/* 将第一个简单命令进程作为进程组组长 */
		if (i == 0)
			setpgid(0, 0);
		/* 子进程 */
		if (cmd[i].infd != 0)
		{
			close(0);
			dup(cmd[i].infd);
		}
		if (cmd[i].outfd != 1)
		{
			close(1);
			dup(cmd[i].outfd);
		}

		int j;
		for (j=3; j<OPEN_MAX; ++j)
			close(j);

		/* 前台作业能够接收SIGINT、SIGQUIT信号 */
		/* 这两个信号要恢复为默认操作 */
		if (backgnd == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
		}
		execvp(cmd[i].args[0], cmd[i].args);
		exit(EXIT_FAILURE);
	}
}
