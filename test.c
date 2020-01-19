#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

char buff[256];


void usage(char *s) {
	char buff[128];
	int len = sprintf(buff, "Este prorgama requiere 2 parametros de entrada\n%s X N\n", s);
	write(1, buff, len);
	exit (1);
}

void error_y_exit(char *msg) {
	perror(msg);
	exit(-1);
}



int main(int argc, char* argv[]) {
    if (argc != 3) usage(argv[0]);
    
    int X = atoi(argv[1]); //Veces que generammos un numero
    int N = atoi(argv[2]); //Rango de los numeros a generar
    
    int fd = open("ocurrencias.bin", O_RDWR|O_CREAT|O_TRUNC, 0664); //Cada ejecucion borrara lo que haya en el fichero si este ya existia
    
    if (fd < 0) error_y_exit("Error en open");
    
    for (int i = 0; i < N; ++i) { //Inicializamos el fichero a 0 con los posibles valores que podemos obtener de ran()%N
        int aux = 0;
        write(fd, &aux, sizeof(int));
    }
    
    for (int i = 0; i < X; ++i) {
        int num = rand()%N;
        if (lseek(fd, num * 4, SEEK_SET) < 0) error_y_exit("Error en lseek"); //Nos posicionamos en el numero generado en el fichero.
        int aux;
        if (read(fd, &aux, sizeof(int)) == -1) error_y_exit("Error en read");
        ++aux;
        if (lseek(fd, -4, SEEK_CUR) < 0) error_y_exit("Error en lseek"); //Escribimos el numero de veces que ha aparecido el valor en el fichero.
        write(fd, &aux, sizeof(aux));
    }
}
