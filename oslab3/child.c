#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>

int fd, status;
float get_msg, s = 0;
float *ptr;


int main(int argc, char *argv[])
{
	fd = open("comm", O_RDWR);
	if (fd < 0)
	{
		printf("Opening a file error\n");
		return 9;
	}
	ptr = mmap(NULL, 2*sizeof(float), PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);  //отображение файла, на который ссылается файловый дескриптор fd, в память процесса                                     //функция mmap возвращает указатель на начало отображенной памяти
	if (ptr == MAP_FAILED)
	{
		printf("MMAP error\n");
		return 10;
	}


	while (1)
	{
		while (ptr[0] == 0.0)
		{
		}
		if (ptr[0] == 1.0)
		{
			get_msg = ptr[1];
			s += get_msg;			
			ptr[0] = 0;
		}
		else //ptr[0] = 2
		{	
			FILE *fout = fopen(argv[0], "w");
			if (fout == NULL)
			{
				printf("File opening error\n");
				return 11;
			}
			if (fprintf(fout, "%f\n", s) <= 0)
			{
				printf("File writing error\n");
				return 12;
			}
			fclose(fout);

			break;
		}
	}
	close(fd);
	status = munmap(ptr, 2*sizeof(float));
	if (status < 0)
	{
		printf("Closing the memory-mapped file error\n");
		return 13;
	}
	return 0;
}
