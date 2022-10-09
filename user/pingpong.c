#include "kernel/types.h"
#include "user/user.h"

int main(int argc,char *argv[]){
    int ptoc[2];
    int ctop[2];
    int pid;
    char buf[4];                                                                                                                   

    if(pipe(ptoc)<0 || pipe(ctop)<0){
         write(2, "create pipe failed\n", 19);
         exit(1);
     }
    pid=fork();
    if(pid){
        close(ptoc[0]);
        close(ctop[1]);
        write(ptoc[1],"ping",4);
        close(ptoc[1]);
        read(ctop[0],buf,4);
        printf("%d: received %s\n",getpid(),buf);
        close(ctop[0]);
    }else{
        close(ptoc[1]);
        close(ctop[0]);
        read(ptoc[0],buf,4);
        close(ptoc[0]);
        printf("%d: received %s\n",getpid(),buf);
        write(ctop[1],"pong",4);      
        close(ctop[1]);
    }
    exit(0);
}                             
