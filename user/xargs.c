// #include "kernel/types.h"
// #include "kernel/stat.h"
// #include "user/user.h"
// #include "kernel/param.h"

// #define MAXN 1024

// int main(int argc, char *argv[])
// {
//     if(argc <= 2){
//         printf("Xargs needs more argument!\n"); //检查参数数量是否正确
//         printf("argc=%d\n", argc);
//         printf("argv=%s, %s, %s\n", argv[0], argv[1], argv[2]);
//         exit(-1);
//     }
//     char* argvs[MAXARG];
//     char buf[MAXN] = {"\0"};
//     int n;
//     for(int i=1; i<argc; i++){
//         argvs[i-1] = argv[i];
//     }
//     while((n = read(0, buf, MAXN)) > 0){
//         char temp[MAXN] = {"\0"};
//         argvs[argc-1] = temp;
//         for(int i = 0; i < strlen(buf); i++){
//             if(buf[i] == '\n'){
//                 if(fork() == 0){
//                     exec(argv[1], argvs);
//                 }
//                 wait(0);
//             }else{
//                 temp[i] = buf[i];
//             }
//         }
//     }
//     exit(0);
// }

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

#define MAXN 1024

int main(int argc, char *argv[])
{
    if(argc <= 2){
        printf("Xargs needs one argument!\n"); //检查参数数量是否正确
        printf("argc=%d\n", argc);
        printf("argv=%s, %s, %s\n", argv[0], argv[1], argv[2]);
        exit(-1);
    }
    char* tempArgvs[MAXARG];
    char buf[MAXN] = {"\0"};
    int n;
    // 创建新数组，删去了原来的xargs，后面的参数前移
    for(int i=1; i<argc; i++){
        tempArgvs[i-1] = argv[i];
    }
    // 依次读取后面参数
    while((n = read(0, buf, MAXN)) > 0){
        char temp[MAXN] = {"\0"};
        tempArgvs[argc-1] = temp;
        for(int i = 0; i < strlen(buf); i++){
            // 按行读取，直到换行符。然后用fork创建子进程，将读取参数传入，执行子程序
            if(buf[i] == '\n'){
                if(fork() == 0){
                    exec(argv[1], tempArgvs);
                }
                wait(0);
            }else{
                temp[i] = buf[i];
            }
        }
    }
    exit(0);
}

