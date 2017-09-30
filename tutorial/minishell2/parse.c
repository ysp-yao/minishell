#include "parse.h"
#include "externs.h"
#include "init.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/*
 * shell��ѭ��
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
		execute_command();
	}
	
	printf("\nexit\n");
}

/*
 * ��ȡ����
 * �ɹ�����0��ʧ�ܻ��߶�ȡ���ļ�������(EOF)����-1
 */
int read_command(void)
{
	if (fgets(cmdline, MAXLINE, stdin) == NULL)
		return -1;
	return 0;
}

/*
 * ��������
 * �ɹ����ؽ����������������ʧ�ܷ���-1
 */
int parse_command(void)
{
	/* ls -l */
	/* ls\0-l\0 */

	char *cp = cmdline;
	char *avp = avline;
	int i = 0;

	while (*cp != '\0')
	{
		/* ȥ���ո� */
		while (*cp == ' ' || *cp == '\t')
			cp++;
		/* ���������β������ѭ�� */
		if (*cp == '\0' || *cp == '\n')
			break;
		cmd.args[i] = avp;

		/* �������� */
		while (*cp != '\0'
			&& *cp != ' '
			&& *cp != '\t'
			&& *cp != '\n')
			*avp++ = *cp++;
		*avp++ = '\0';
		printf("[%s]\n", cmd.args[i]);
		i++;
	}

	return 0;
}

/*
 * ִ������
 * �ɹ�����0��ʧ�ܷ���-1
 */
int execute_command(void)
{
	pid_t pid = fork();
	if (pid == -1)
		ERR_EXIT("fork");

	if (pid == 0)
		execvp(cmd.args[0], cmd.args);

	wait(NULL);
	return 0;
}
