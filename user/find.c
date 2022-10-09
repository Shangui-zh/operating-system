// #include "kernel/types.h"
// #include "user/user.h"
// #include "kernel/stat.h"
// #include "kernel/fs.h"

// void find(char *dir, char *file)
// {
//     char buf[512], *p;
//     int fd;
//     struct dirent de;
//     struct stat st;

//     // open()打开路径，返回文件描述符，如果错误返回-1
//     if((fd = open(dir, 0)) < 0){
//         // fprintf(2, "find: cannot open %s\n", dir);
//         printf("find: cannot open %s\n", dir);
//         return;
//     }
//     // 由文件描述词，获得文件状态，复制到参数所指的结构中
//     if(fstat(fd, &st) < 0){
//         // fprintf(2, "find: cannot stat %s\n", dir);
//         printf("find: cannot stat %s\n", dir);
//         close(fd);
//         return;
//     }
//     // 如果不是目录类型
//     if(st.type != T_DIR){
//         // fprintf(2, "find: %s is not a directory\n", dir);
//         printf("find: %s is not a directory\n", dir);
//         close(fd);
//         return;
//     }
//     // 如果路径过长无法放入缓冲区
//     if(strlen(dir) + 1 + DIRSIZ + 1 > sizeof(buf)){
//         // fprintf(2, "find: directory too long\n");
//         printf("find: directory too long\n");
//         close(fd);
//         return;
//     }
//     strcpy(buf, dir);
//     p = buf + strlen(buf);
//     *p++ = '/';
//     // 读取fd，如果read返回字节数与de长度相等则循环
//     while(read(fd, &de, sizeof(de)) == sizeof(de)){
//         if(de.inum == 0) continue;
//         if(!strcmp(de.name, ".") || !strcmp(de.name, "..")) continue;
//         // 把de.name信息复制到p
//         memmove(p, de.name, DIRSIZ);
//         // 设置文件名结束符
//         p[DIRSIZ] = 0;
//         if(stat(buf, &st) < 0){
//             // fprintf(2, "find: cannot stat %s\n", buf);
//             printf("find: cannot stat %s\n", buf);
//             continue;
//         }
//         // 如果目录类型，递归查找
//         if(st.type == T_DIR){
//             find(buf, file);
//         }else if(st.type == T_FILE && !strcmp(de.name, file)){
//             // 如果文件类型且名称相同
//             printf("%s\n", buf);
//         }
//     }
// }

// int main(int argc, char *argv[]){
//     if(argc != 3){
//         printf("usage: find dirName fileName\n");
//         printf("argc=%d\n", argc);
//         printf("argv=%s, %s, %s\n", argv[0], argv[1], argv[2]);
//         exit(1);
//     }
//     find(argv[1], argv[2]);
//     exit(0);
// }
#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"
#include "kernel/fs.h"

void find(char *dir, char *file)
{
    char buf[512], *temp;
    int fd1;
    struct stat fileSt;
    struct dirent fileDe;
    

    // // 判断目录
    // if((fd1 = open(dir, 0)) < 0){
    //     printf("find: cannot open %s\n", dir);
    //     return;
    // }
    // // 判断文件
    // if(fstat(fd1, &fileSt) < 0){
    //     printf("find: cannot stat %s\n", dir);
    //     close(fd1);
    //     return;
    // }
    // // 如果不是目录类型
    // if(fileSt.type != T_DIR){
    //     printf("find: %s is not a directory\n", dir);
    //     close(fd1);
    //     return;
    // }
    // // 如果路径过长无法放入缓冲区
    // if(strlen(dir) + 1 + DIRSIZ + 1 > sizeof(buf)){
    //     printf("find: directory too long\n");
    //     close(fd1);
    //     return;
    // }
    // 判断目录
    if(((fd1 = open(dir, 0)) < 0) || (fstat(fd1, &fileSt) < 0) || (fileSt.type != T_DIR) || (strlen(dir) + 1 + DIRSIZ + 1 > sizeof(buf))){
        printf("find: cannot open %s\n", dir);
        close(fd1);
        return;
    }
    // // 判断文件
    // if(fstat(fd1, &fileSt) < 0){
    //     printf("find: cannot stat %s\n", dir);
    //     close(fd1);
    //     return;
    // }
    // // 如果不是目录类型
    // if(fileSt.type != T_DIR){
    //     printf("find: %s is not a directory\n", dir);
    //     close(fd1);
    //     return;
    // }
    // // 如果路径过长无法放入缓冲区
    // if(strlen(dir) + 1 + DIRSIZ + 1 > sizeof(buf)){
    //     printf("find: directory too long\n");
    //     close(fd1);
    //     return;
    // }
    strcpy(buf, dir);
    temp = buf + strlen(buf);
    *temp++ = '/';
    // 读取fd，如果read返回字节数与fileDe长度相等则循环
    while(read(fd1, &fileDe, sizeof(fileDe)) == sizeof(fileDe)){
        if(!strcmp(fileDe.name, "..") || !strcmp(fileDe.name, ".") || (fileDe.inum == 0)) continue;
        memmove(temp, fileDe.name, DIRSIZ);
        temp[DIRSIZ] = 0;
        if(stat(buf, &fileSt) < 0){
            printf("find: cannot stat %s\n", buf);
            continue;
        }
        // 如果目录类型，递归查找
        if(fileSt.type == T_DIR){
            find(buf, file);
        }else if(fileSt.type == T_FILE && !strcmp(fileDe.name, file)){
            // 如果文件类型且名称相同
            printf("%s\n", buf);
        }
    }
}

int main(int argc, char *argv[]){
    if(argc != 3){
        printf("usage: find dirName fileName\n");
        printf("argc=%d\n", argc);
        printf("argv=%s, %s, %s\n", argv[0], argv[1], argv[2]);
        exit(1);
    }
    find(argv[1], argv[2]);
    exit(0);
}