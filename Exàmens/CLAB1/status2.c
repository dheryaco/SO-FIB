#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

void error_y_exit(char *msg)
{
	perror(msg);
	exit(1);
}

void usage()
{
	char buff[256];
	
	sprintf(buff, "Usage: status pid1 .. pidn\n");
	write(1, buff, strlen(buff));
	exit(1);
}

int main(int argc, char *argv[])
{
	int i, pid;
	char arg[256];
	int status;
	int vpid[100];
	
	if (argc < 2)
		usage();
		
	for (i = 1; i < argc; i++) {
		vpid[i-1] = fork();
		if (vpid[i-1] < 0)
			error_y_exit("fork");
		else if (vpid[i-1] == 0) {
			sprintf(arg, "/proc/%s/status", argv[i]);
			execlp("grep", "grep", "State", arg, (char *)0);
			error_y_exit("execlp");
		}
	}

	while ((pid = waitpid(-1, &status, 0)) > 0) {
		if (WIFEXITED(status)) {
			if (WEXITSTATUS(status) != 0) {
				i = 0;				
				while (pid != vpid[i]) 
					i++;
				sprintf(arg, "Proceso %s no existe\n", argv[i+1]);
				write(1, arg, strlen(arg));
				exit(1);				
			}
		}
	}
}

