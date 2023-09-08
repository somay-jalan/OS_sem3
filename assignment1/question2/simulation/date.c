#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<time.h>
int main(int argc,char *argv[],char *envp[]){
    struct stat filestat;
    char open_file[200];
    int end1=0;
    for (int i=0;i<100;i++){
        if(argv[2][i]=='\0'){
            end1=i;
            break;
        }
        open_file[i]=argv[2][i];
    }
    open_file[end1]='/';
    end1++;
    for(int i=0;i<100;i++){
        open_file[i+end1]=argv[1][i];
        if (argv[1][i]=='\0'){
            end1+=i;
            break;
        }
    }
    FILE *f=fopen(open_file,"r");
    if(f==NULL){
        printf("File doesnt exist\n");
        return 0;
    }
    stat(open_file,&filestat);
    printf("%s",ctime(&filestat.st_mtime));
    return 0;
}