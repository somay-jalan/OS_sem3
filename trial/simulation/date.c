#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/stat.h>

int main(){
    char *fd = "test.txt";
    struct stat buf;          

    stat(fd, &buf);
    int time = buf.st_mtime;

    printf("%d\n",time);
    return 0;
}