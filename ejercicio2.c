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
void manejadora(int num_sig){
    wait(0);
    signal(num_sig,manejadora);
}

int main(int argc, char * argv[]){

    if(argc < 2){
        printf("Argumentos insuficientes\n");
        exit(1);
    }

    int fd_pipe[2];
    int fd_pipe2[2];
    int limite = atoi(argv[1]);
    
    int e =  pipe(fd_pipe);
    int r = pipe(fd_pipe2);

    pid_t pid = fork();
    if(pid < 0){
        perror("Error al parir\n");
        exit(1);
    }


    if(pid == 0){

        close(fd_pipe2[0]);
        close(fd_pipe[1]);
        //char cadena[TAM];
        int length;
        int pos = 0;
        int leido;
        char ok[] = "y";
        char bad[] = "n";

        do{
            char cadena[TAM];
            leido = read(fd_pipe[0],&length,4);
            if(leido < 0){
                perror("Error el leer\n");
                exit(1);
            }

            leido = read(fd_pipe[0],&cadena,length);
            if(leido < 0){
                perror("Error al leer\n");
                exit(1);
            }
            
            pos += leido;
            cadena[length] = '\0';

            printf("MI papa me ha enviado esto: %s Pos:%d\n ",cadena,pos);

            if(pos < limite){
                int env = write(fd_pipe2[1],&ok,1);
                if(env < 0){
                    perror("Error\n");
                    exit(1);
                }

            }else{
                int env = write(fd_pipe2[1],&bad,1);
                if(env < 0){
                    perror("Error\n");
                    exit(1);
                }
            }
            
        }while(pos < limite && leido > 0);

            
        
        close(fd_pipe[0]);
        close(fd_pipe2[1]);
        exit(0);
        
    }else{

        close(fd_pipe[0]);
        close(fd_pipe2[1]);
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

            int escrito = write(fd_pipe[1],&length,4);
            if(escrito <0){
                perror("Error el enviar cabecera\n");
                exit(1);
            }


            escrito = write(fd_pipe[1], &cadena, strlen(cadena));
            if(escrito < 0){
                perror("Error al escribir el payload\n");
            }

            int red = read(fd_pipe2[0],&o,1);
            if(red < 0){
                perror("Error\n");
                exit(1);   
            }
        }while((strstr(cadena,"fin") == NULL) && o != 'n');

        
        close(fd_pipe[1]);
    }
    return 0;
}