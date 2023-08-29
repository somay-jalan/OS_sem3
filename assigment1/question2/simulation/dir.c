#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/wait.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc,char *argv[],char *envp[]){
    // printf("r:%c v:%c dirname:%s curr:%s\n",argv[0][0],argv[0][1],argv[1],argv[2]);
    struct dirent *dir_cur;
    DIR *dr = opendir(argv[2]);
    if (dr == NULL){
        printf("Could not open current directory" );
        return 0;
    }
    while ((dir_cur = readdir(dr)) != NULL){
        if(strcmp(argv[1],dir_cur->d_name)==0){
            if(argv[0][0]=='b'){
                char remove_dir[200];
                int end1=0;
                for (int i=0;i<100;i++){
                    if(argv[2][i]=='\0'){
                        end1=i;
                        break;
                    }
                    remove_dir[i]=argv[2][i];
                }
                remove_dir[end1]='/';
                end1++;
                for(int i=0;i<100;i++){
                    remove_dir[i+end1]=argv[1][i];
                    if (argv[1][i]=='\0'){
                        end1+=i;
                        break;
                    }
                }
                if(argv[0][1]=='b'){
                    printf("%s is being removed in ~/%s\n",argv[1],argv[2]);
                }
                rmdir(remove_dir);
            }else{
                printf("Directory already exists\n");
                FILE *file_wirte=fopen("write.txt","w+");
                fprintf(file_wirte,"%s",argv[2]);
                return 0;
            }
        }
    }
    closedir(dr); 
    char make_dir[200];
    int end1=0;
    for (int i=0;i<100;i++){
        if(argv[2][i]=='\0'){
            end1=i;
            break;
        }
        make_dir[i]=argv[2][i];
    }
    make_dir[end1]='/';
    end1++;
    for(int i=0;i<100;i++){
        make_dir[i+end1]=argv[1][i];
        if (argv[1][i]=='\0'){
            end1+=i;
            break;
        }
    }
    if(argv[0][1]=='b'){
        printf("%s is being created in ~/%s\n",argv[1],argv[2]);
        printf("Directory is being swtiched to ~/%s\n",make_dir);
    }
    mkdir(make_dir,0755);
    FILE *file_wirte=fopen("write.txt","w+");
    fprintf(file_wirte,"%s",make_dir);
    return 0;
}