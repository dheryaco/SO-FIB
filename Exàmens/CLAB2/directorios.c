#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

char buff[256];

int fd[2];

void usage(char *s) {
	char buff[128];
	sprintf(buff, "Este prorgama no requiere parametros de entrada\n%s\n", s);
	write(1, buff, strlen(buff));
	exit (1);
}

void error_y_exit(char *msg) {
	perror(msg);
	exit(-1);
}

int main(int argc, char* argv[]) {
    if (argc != 1) usage(argv[0]);
    
    if (pipe(fd) < 0) error_y_exit("Error en pipe");
    
    int pid;
    pid = fork();
    if (pid < 0) error_y_exit("Error en fork");
    else if (pid == 0) {
        if (close(fd[1]) == -1) error_y_exit("Error en close"); //Cerramos el canal de escritura de la pipe en el hijo ya que este escribe por el   
                                                                //canal estandar de salida
        
        if (dup2(fd[0], 0) == -1) error_y_exit("Error en dup2"); //Duplicamos el canal de lectura de la pipe en el hijo en el canal estandar para  
        if (close(fd[0]) == -1) error_y_exit("Error en close"); //que este pueda leer la informacion que escribe su padre.
        
        execlp("sort", "sort", "-nr", NULL);
        error_y_exit("Error en execlp");
    }
    else {
        if (close(fd[0]) == -1) error_y_exit("Error en close"); //Cerramos el canal de lectura de la pipe en el padre ya que este no lo necesita
        
        if (dup2(fd[1], 1) == -1) error_y_exit("Error en dup2"); //Duplicamos el canal de escritura de la pipe en el canal estandar de salida del  
        if (close(fd[1]) == -1) error_y_exit("Error en close");  //padre.
        
        execlp("du","du", "-b", "/home", NULL);
        error_y_exit("Error en execlp");
    }
}
