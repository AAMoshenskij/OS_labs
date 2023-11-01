#include <stdio.h>
#include <unistd.h>
#include <string.h>

int fd[2];
int fd1[2];
int pres, pres1;

int main(int argc, char *argv[])
{
	printf("banana\n");
	if (argc != 2)
	{
		printf("2 arguments expected\n");
		return 1;
	}
	else if (strlen(argv[1]) > 100)
	{
		printf("Incorrect argument\n");
		return 2;
	}
	pres = pipe(fd);                              //создаем 2 однонапрвленных канала
	pres1 = pipe(fd1);
	if ((pres < 0) || (pres1 < 0))
	{
		printf("Pipe creation error\n");
		return 3;
	}
	printf("1 Read File Descriptor Value: %d\n", fd[0]);
	printf("1 Write File Descriptor Value: %d\n", fd[1]);
	printf("2 Read File Descriptor Value: %d\n", fd1[0]);
	printf("2 Write File Descriptor Value: %d\n", fd1[1]);
	
	int id = fork();                                         //создаем дочерний процесс
	if (id < 0)
	{
		printf("Fork error\n");
		return 4;
	}

	else if (id > 0)
	{
		close(fd[0]);
		close(fd1[1]);
		printf("parent\n");
		int n = strlen(argv[1]);
		write(fd[1], &n, sizeof(n));
		write(fd[1], argv[1], sizeof(argv[1]));
		float send_msg;
		while (scanf("%f", &send_msg) == 1)
		{
			if (write(fd[1], &send_msg, sizeof(send_msg)) <= 0)
			{
				printf("Communication with child process error\n");
				return 5;
			}
		}
		close(fd[1]);
		int exc;
		if (read(fd1[0], &exc, sizeof(exc)) <= 0)
		{
			printf("Communication with child process error\n");
			return 5;
		}
		if (exc != 0)
		{
			printf("Fail\n");
			return exc;
		}
		close(fd1[0]);
	}
	
	else
	{
		close(fd[1]);
		close(fd1[0]);
		char s[100], s1[100];
		if ((sprintf(s, "%d", fd[0]) <= 0) || (sprintf(s1, "%d", fd1[1]) <= 0))
		{
			printf("Descriptor conversion error\n");
			return 6;
		}
//		printf("%s\n", s);
//		printf("%s\n", s1);
		if (execl("b.out", s, s1, (char *) NULL) < 0)
		{
			printf("Child process loading error\n");
			return 7;
		}
		
	}
	return 0;
}
