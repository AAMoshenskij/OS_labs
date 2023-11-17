#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>


int fd, status, id;
float send_msg;
float *ptr;

//ptr[0]:
//0 - not written, not ready for reading
//1 - written, ready for reading
//2 - end this

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("2 arguments expected\n");
		return 1;
	}
	else if (strlen(argv[1]) > 255)
	{
		printf("Incorrect argument\n");
		return 2;
	}

	fd = open("comm", O_RDWR | O_CREAT | O_TRUNC, 0777);
	if (fd < 0)
	{
		printf("Opening a file error\n");
		return 3;
	}
	status = ftruncate(fd, 2*sizeof(float));       //обрезание файла до размера 2*sizeof(float)
	if (status < 0)
	{
		printf("Changing a length of the file error\n");
		return 4;
	}
	ptr = mmap(NULL, 2*sizeof(float), PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);   //отображение файла, на который ссылается файловый дескриптор fd, в память процесса                                     //функция mmap возвращает указатель на начало отображенной памяти
	if (ptr == MAP_FAILED)
	{
		printf("MMAP error\n");
		return 5;
	}

	ptr[0] = 0.0;

	id = fork();
	if (id < 0)
	{
		printf("Fork error\n");
		return 6;
	}
	else if (id > 0)
	{
		while (1)
		{
			while (ptr[0] == 1)              //цикл ожидает, пока основной процесс не укажет, что данные готовы для чтения
			{
			}
			status = scanf("%f", &send_msg);
			if (status > 0)
			{
				ptr[1] = send_msg;
				ptr[0] = 1;
			}
			else
			{
				printf("End\n");
				ptr[0] = 2.0;
				break;
			}
		}
		close(fd);
		status = munmap(ptr, 2*sizeof(float));   //отключение отображения области памяти, на которую указывает ptr
		if (status < 0)
		{
			printf("Closing the memory-mapped file error\n");
			return 7;
		}
	}
	else
	{
		if (execl("b.out", argv[1], (char *) NULL) < 0)
		{
			printf("Child process loading error\n");
			return 8;
		}
		
	}
	return 0;
}
