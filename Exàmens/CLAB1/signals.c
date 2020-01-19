#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int alarm_recibido = 0;

void error_y_exit(char *msg)
{
	perror(msg);
	exit(1);
}

void usage()
{
	char buff[256];
	
	sprintf(buff, "Usage: signals n\n");
	write(1, buff, strlen(buff));
	exit(1);
}

void trat_alarm(int s)
{
	alarm_recibido = 1;
}

void trat_usr1(int s)
{
}

int main(int argc, char *argv[])
{
	int n, i, pid;
	char buff[256];
	sigset_t mask;
	struct sigaction trat;
	
	if (argc < 2)
		usage();
	
	trat.sa_handler = trat_alarm;
	sigemptyset(&trat.sa_mask);
	trat.sa_flags = 0;
	if (sigaction(SIGALRM, &trat, NULL) < 0)
		error_y_exit("sigaction");

	trat.sa_handler = trat_usr1;
	if (sigaction(SIGUSR1, &trat, NULL) < 0)
		error_y_exit("sigaction");
		
	sigemptyset(&mask);
	sigaddset(&mask, SIGUSR1);
	if (sigprocmask(SIG_BLOCK, &mask, NULL) < 0)
		error_y_exit("sigprocmask");

	n = atoi(argv[1]);

	for (i = 0; i < n; i++) {
		
		pid = fork();
		
		if (pid < 0)
			error_y_exit("fork");
			
		else if (pid == 0) {
			sprintf(buff, "Hijo %d creado\n", getpid());
			write(1, buff, strlen(buff));
			alarm(1);
			while (!alarm_recibido);
			if (kill(getppid(), SIGUSR1) < 0)
				error_y_exit("kill");
			exit(1);	
		}
		
		else {
			sigfillset(&mask);
			sigdelset(&mask, SIGUSR1);
			sigsuspend(&mask);
			
			if (waitpid(-1, NULL, 0) < 0)
				error_y_exit("waitpid");
		}
	}
	

}


