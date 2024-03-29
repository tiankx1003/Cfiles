/* ex10.c */
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	int n,fd[2];
	pid_t pid;
	char buffer[BUFSIZ+1];
	FILE *fp;

	if(argc<=1)
	{
		printf("usage: %s <pathname>\n",argv[0]);
		exit(1);
	}
	if((fp=fopen(argv[1],"r"))==NULL)
	{
		printf("Can't open %s \n", argv[1]);
		exit(1);
	}
	if(pipe(fd)<0)
	{
		printf("pipe failed!\n ");
		exit(1);
	}
	if((pid=fork())<0)
	{
		printf("fork failed!\n ");
		exit(1);
	}
	else if (pid>0)
	{
		close(fd[0]);
		while(fgets(buffer,BUFSIZ,fp)!=NULL)
		{
			n=strlen(buffer);
			if(write(fd[1],buffer,n)!=n)
			{
				printf("write error to pipe.\n");
				exit(1);
			}
		}
		if(ferror(fp))
		{
			printf("fgets error. \n");
			exit(1);
		}
		close(fd[1]);
		if(waitpid(pid, NULL, 0)<0)
		{
			printf("waitpid error!\n");
			exit(1);
		}
		exit(0);
	}
	else
	{
		close(fd[1]);
		if(fd[0]!=STDIN_FILENO)
		{
			if(dup2(fd[0],STDIN_FILENO)!=STDIN_FILENO)
			{
				printf("dup2 error to stdin! \n");
				exit(1);
			}
			close(fd[0]);
		}
		if(execl("upcase","upcase",(char *)0)<0)
		{
			printf("execl error for upcase.\n");
			exit(1);
		}
		exit(0);
	}
}
