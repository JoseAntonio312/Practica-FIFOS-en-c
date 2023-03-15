//HACER O MISMO QUE EL 2 PERO CON FIFOS
//HACER LOS MISMO QUE EL2 PERO CON FIFOS
#include<stdio.h>
#include<sys/time.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<signal.h>
#include<sys/wait.h>
#define TAM 500


int main(){
    
    
    int op= open("mififo",O_WRONLY);
    int op2 = open("mififo2",O_RDONLY);

    char cadena[TAM];
        char o;
    
        do{
            int leidos = read(0,cadena,500);
            if (leidos < 0){
                perror("Ha ocrrido un error al leer\n");
                exit(1);
            }

            cadena[leidos] = '\0';

            int length = strlen(cadena);

            int escrito = write(op,&length,4);
            if(escrito <0){
                perror("Error el enviar cabecera\n");
                exit(1);
            }


            escrito = write(op, &cadena, strlen(cadena));
            if(escrito < 0){
                perror("Error al escribir el payload\n");
            }

            int red = read(op2,&o,1);
            if(red < 0){
                perror("Error\n");
                exit(1);   
            }
        }while((strstr(cadena,"fin") == NULL) && o != 'n');

        close(op);
        close(op2);
    return 0;
}