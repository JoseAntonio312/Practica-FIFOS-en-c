#include<stdio.h>
#include<sys/time.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include <sys/stat.h>
#include <fcntl.h>

ssize_t readn(int fd, void * datos,size_t n){
    size_t intentandoLeer = n;
    size_t leidoTotal = 0;
    char * p = (char*)datos;
    ssize_t leido;

    do{
        errno =0;
        leido = read(fd,p+leidoTotal,intentandoLeer);
        if(leido > 0){
            intentandoLeer -= leido;
            leidoTotal += leido;
        }
    }while((leido > 0 && leidoTotal < n) || errno == EINTR);

    if(leido < 0){
        return leido;
    }else{
        return leidoTotal;
    }
}


int main(){

    u_int8_t tipo;
    u_int16_t operando1 ;
    u_int16_t operando2 ;

    int fd = open("mififo",O_RDONLY);
    if(fd < 0){
        perror("Error en la apertura de la fifo\n");
        exit(1);
    }

    //Leemos lo que nos envía el escritor
    int rec = readn(fd,&tipo,1);
    if(rec < 0){
        perror("Error al leer\n");
        exit(1);
    }

     rec = readn(fd,&operando1,2);
    if(rec < 0){
        perror("Error al leer\n");
        exit(1);
    }

     rec = readn(fd,&operando2,2);
    if(rec < 0){
        perror("Error al leer\n");
        exit(1);
    }

    int resultado;
    //Calcula el resultado --- NO RAYARSE CON ESO
    if(tipo == 1){
        resultado = operando1 + operando2;
    }else{
        resultado = operando1 * operando2;
    }

    printf("EL resultado de lo que nos ha enviado nuestro escritor ha sido : %d\n",resultado);
}