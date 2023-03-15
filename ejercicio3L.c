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


int main(int argc, char* argv[]){

    if(argc < 2){
        printf("Argumentos insuficientes\n");
        exit(1);
    }

    int limite = atoi(argv[1]);

    int op = open("mififo",O_RDONLY);
    int op2 = open("mififo2",O_WRONLY);

    int length;
        int pos = 0;
        int leido;
        char ok[] = "y";
        char bad[] = "n";

        do{
            char cadena[TAM];
            leido = read(op,&length,4);
            if(leido < 0){
                perror("Error el leer\n");
                exit(1);
            }

            leido = read(op,&cadena,length);
            if(leido < 0){
                perror("Error al leer\n");
                exit(1);
            }
            
            pos += leido;
            cadena[length] = '\0';

            printf("MI papa me ha enviado esto: %s Pos:%d\n ",cadena,pos);

            if(pos < limite){
                int env = write(op2,&ok,1);
                if(env < 0){
                    perror("Error\n");
                    exit(1);
                }

            }else{
                int env = write(op2,&bad,1);
                if(env < 0){
                    perror("Error\n");
                    exit(1);
                }
            }
            
        }while(pos < limite && leido > 0);

        close(op);
        close(op2);

    return 0;
}