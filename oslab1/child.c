#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int fd[2], fd1[2];
int exc = 0;

int main(int argc, char *argv[])
{
	printf("baobab\n");
	
//	printf("%s\n", argv[0]);
//	printf("%s\n", argv[1]);
	fd[0] = atoi(argv[0]);
	fd1[1] = atoi(argv[1]);
	if (fd[0] <= 0 || fd1[1] <= 0)
	{
		printf("Descriptor reverse conversion error\n");
		exc = 8;
		return 8;
	}
	//printf("%d\n", fd[0]);
	//printf("%d\n", fd1[1]);
	
	int n;
	if (read(fd[0], &n, sizeof(n)) <= 0)
	{
		printf("Reading error in child process\n");
		exc = 9;
		return 9;
	}
	char file_out[100];
	if (read(fd[0], file_out, n) <= 0)
	{
		printf("Reading error in child process\n");
		exc = 9;
		return 9;
	}
	printf("%s\n", file_out);
	float get_msg, s = 0;
	while (read(fd[0], &get_msg, sizeof(get_msg)) != 0)
	{
//		printf("%f\n", get_msg);		
		s += get_msg;
//		printf("%f\n", s);
	}
	close(fd[0]);
	FILE *fout = fopen(file_out, "w");
	if (fout == NULL)
	{
		printf("File opening error\n");
		exc = 10;
		return 10;
	}
	if (fprintf(fout, "%f\n", s) <= 0)
	{
		printf("File writing error\n");
		exc = 11;
		return 11;
	}
	fclose(fout);
	close(fd[0]);
	write(fd1[1], &exc, sizeof(exc));
	close(fd1[1]);

	return 0;
}
