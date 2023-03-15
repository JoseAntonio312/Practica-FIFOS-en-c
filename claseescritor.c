#include<stdio.h>
#include<sys/time.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include <sys/stat.h>
#include <fcntl.h>

ssize_t writen(int fd, void * datos, size_t n){
    size_t intentadoEscribir = n;
    size_t totalEscritos = 0;
    char * p = (char*)datos;
    ssize_t escrito;//Write

    do{
        errno = 0;
        escrito = write (fd, p+totalEscritos,intentadoEscribir);
        if(escrito > 0){
            intentadoEscribir -= escrito;
            totalEscritos += escrito;
        }

    }while( escrito > 0 && totalEscritos < n || errno == EINTR);

    if(escrito < 0){
        return escrito;
    }else{
        return totalEscritos;
    }
}

int main(int argc , char* argv[]){

    if(argc < 4){
        printf("Argumentos insuficientes: %d\n",argc);
        exit(1);
    }

    u_int8_t tipo = atoi(argv[1]);
    u_int16_t operando1 = atoi(argv[2]);
    u_int16_t operando2 = atoi(argv[3]);

    int fd = open("mififo",O_WRONLY);
    if(fd < 0){
        perror("Error al abrir la fifo\n");
        exit(1);
    }



    //Enviamos todo
    int env = writen(fd,&tipo,1);
    if(env < 0){
        perror("Error en writen\n");
        exit(1);
    }

    env = writen(fd,&operando1,2);
    if(env < 0){
        perror("Error en writen\n");
        exit(1);
    }

    env = writen(fd,&operando2,2);
    if (env < 0){
        perror("Error en writen\n");
        exit(1);
    }


    printf("HE terminaod mi ejecución\n");

    close(fd);
}