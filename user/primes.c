#include "kernel/types.h"
#include "user/user.h"

void prime(int *pd_in){
    int pd_out[2];
    int num_in1,num_in2;
    close(pd_in[1]);

    if(pipe(pd_out)<0){
        write(2, "create pipe failed\n", 17);
		exit(1);
    }

    if(read(pd_in[0], &num_in1, 4)){
        printf("prime %d\n",num_in1);
    }else exit(0);
    if(read(pd_in[0], &num_in2, 4)){
        int pid = fork();
        if(pid){
            close(pd_out[0]);
            write(pd_out[1], &num_in2, 4);
            while(read(pd_in[0], &num_in2, 4)){
                if(num_in2%num_in1){
                    write(pd_out[1], &num_in2, 4);
                }
            }
			close(pd_out[1]);
			close(pd_in[0]);
            wait(0);
        }else{
            close(pd_in[0]);
            prime(pd_out);
        }
    }else exit(0);
}

int main(int argc, char*argv[])
{
    int pd[2];
	
	if(pipe(pd)<0){
		write(2, "create pipe failed\n", 17);
		exit(1);
	}

    if(fork()){
        close(pd[0]);
        for(int i=2;i<=35;i++)
            write(pd[1], &i, 4);
        close(pd[1]);
        wait(0);
    }else{
        prime(pd);
    }
	exit(0);
}

// #include "kernel/types.h"
// #include "user.h"
// #include <stddef.h>

// void mapping(int n, int fd[])
// {
//     close(n);//关闭文件描述符n，令n映射到fd[n]
//     dup(fd[n]);
//     close(fd[0]);
//     close(fd[1]);
// }

// void primes()
// {
//     int fd[2];
//     pipe(fd);
//     int prime;//当前的质数
//     int ref = read(0, &prime, sizeof(int));
//     if(ref == 0)return;//没有质数了
//     printf("prime %d\n", prime);
//     int pid = fork();
//     if(pid == 0)
//     {
//         int num;
//         mapping(1, fd);//将管道映射到1上
//         while(read(0,&num, sizeof(int)))
//         {
//             if(num%prime == 0)continue;
//             write(1, &num, sizeof(int));
//         }
//     }
//     else 
//     {
//         wait(NULL);
//         mapping(0, fd);//将管道映射到0上
//         primes();
//     }
// }

// int main(int argc,char* argv[])
// {
//     int fd[2];
//     pipe(fd);//父进程写入，子进程读取
//     int pid = fork();
//     if(pid<0)
//     {
//         printf("error!");
//     }
//     else if(pid == 0)
//     {
//         /*子进程 */
//         mapping(1,fd);
//         for(int i = 2;i <= 35; i++)//将所有数字塞入管道
//             write(1, &i, sizeof(int));
//     }
//     else{
//         /*父进程*/ 
//         wait(NULL);//等待子进程结束
//         mapping(0, fd);
//         primes();
//     }
//     exit(0);
// }


