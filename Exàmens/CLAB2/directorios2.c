#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

char buff[256];


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
    
    if (mknod("PIPE_DIR", S_IFIFO|0664, 0) < 0) {           //Creamos la pipe con nombre
        if (errno != EEXIST) error_y_exit("Error en mknod");//Si el error es que ya esta creada entonces seguimos, sino exit();
    }
    
    int pid;
    pid = fork();
    if (pid < 0) error_y_exit("Error en fork");
    else if (pid == 0) {
        int fd_h;
        fd_h = open("PIPE_DIR", O_RDONLY);                         //El hijo ejecuta el open de la pipe en modo READ_ONLY ya que este
        if (fd_h == -1) error_y_exit("Error en el open");          //solo tiene que leer y duplica el canal de la pipe en el canal de lectura 
        if (dup2(fd_h, 0) == -1) error_y_exit("Error en el dup2"); //en el canal de lectura estandar.
        if (close(fd_h) == -1) error_y_exit("Error en close");
        
        execlp("sort", "sort", "-nr", NULL);
        error_y_exit("Error en execlp");
    }
    
    else {
        int fd_p;
        fd_p = open("PIPE_DIR", O_WRONLY);                      //El padre en cambio, hace el open de la pipe en modo WRITE_ONLY ya que este solo quiere                       
        if (fd_p == -1) error_y_exit("Error en el open");       //enviar informacion y duplica el canal de la pipe en el canal de salida estandar.
        if (dup2(fd_p, 1) == -1) error_y_exit("Error en dup2"); 
        if (close(fd_p) == -1) error_y_exit("Error en close"); 
        
        execlp("du","du", "-b", "/home", NULL);
        error_y_exit("Error en execlp");
    }
}
