#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

int word(int d,int n,char filename1[],char filename2[],char cur_direc[]){
    if (d==0 && n==0){
        FILE* ptr;
        char ch;
        char open_file[200];
        int end1=0;
        for (int i=0;i<100;i++){
            if(cur_direc[i]=='\0'){
                end1=i;
                break;
            }
            open_file[i]=cur_direc[i];
        }
        open_file[end1]='/';
        end1++;
        for(int i=0;i<100;i++){
            open_file[i+end1]=filename1[i];
            if (filename1[i]=='\0'){
                end1+=i;
                break;
            }
        }
        ptr=fopen(open_file,"r");
        if(ptr==NULL){
            printf("file doesn't exist\n");
            return 0;
        }
        int count=0;
        while(1){
            ch=fgetc(ptr);
            if(ch==EOF){
                break;
            }
            count++;
        }
        fclose(ptr);
        printf("%d\n",count);
    }else if (n==1){
        FILE* ptr;
        char ch;
        char open_file[200];
        int end1=0;
        for (int i=0;i<100;i++){
            if(cur_direc[i]=='\0'){
                end1=i;
                break;
            }
            open_file[i]=cur_direc[i];
        }
        open_file[end1]='/';
        end1++;
        for(int i=0;i<100;i++){
            open_file[i+end1]=filename1[i];
            if (filename1[i]=='\0'){
                end1+=i;
                break;
            }
        }
        ptr=fopen(open_file,"r");
        if(ptr==NULL){
            printf("file doesn't exist\n");
            return 0;
        }
        int count=0;
        while(1){
            ch=fgetc(ptr);
            if(ch==EOF){
                break;
            }
            if(ch!='\n'){
                count++;
            }
        }
        if(feof(ptr)){
        }
        fclose(ptr);
        printf("%d\n",count);
    }else if(d==1){
        FILE* ptr;
        char ch;
        char open_file[200];
        int end1=0;
        for (int i=0;i<100;i++){
            if(cur_direc[i]=='\0'){
                end1=i;
                break;
            }
            open_file[i]=cur_direc[i];
        }
        open_file[end1]='/';
        end1++;
        for(int i=0;i<100;i++){
            open_file[i+end1]=filename1[i];
            if (filename1[i]=='\0'){
                end1+=i;
                break;
            }
        }
        ptr=fopen(open_file,"r");
        if(ptr==NULL){
            printf("file doesn't exist\n");
            return 0;
        }
        int count1=0;
        while(1){
            ch=fgetc(ptr);
            if(ch==EOF){
                break;
            }
            count1++;
        }
        fclose(ptr);
        open_file[200];
        end1=0;
        for (int i=0;i<100;i++){
            if(cur_direc[i]=='\0'){
                end1=i;
                break;
            }
            open_file[i]=cur_direc[i];
        }
        open_file[end1]='/';
        end1++;
        for(int i=0;i<100;i++){
            open_file[i+end1]=filename2[i];
            if (filename2[i]=='\0'){
                end1+=i;
                break;
            }
        }
        ptr=fopen(open_file,"r");
        if(ptr==NULL){
            printf("file doesn't exist\n");
            return 0;
        }
        int count2=0;
        while(1){
            ch=fgetc(ptr);
            if(ch==EOF){
                break;
            }
            count2++;
        }
        if(feof(ptr)){
        }
        fclose(ptr);
        printf("%d\n",abs(count1-count2));
    }
    return 0;
}

int dir(int r,int v ,char dir_name[],char cur_direc[]){
    int rc=fork();
    if(rc>0){
        wait(NULL);
    }else{
        char *date_run[]={"./dir",NULL};
        execv(date_run[0],date_run);
        exit(0);
    }
    return 0;
}

int date(int R, int d,char filename[],char cur_direc[]){
    int rc=fork();
    if(rc>0){
        wait(NULL);
    }else{
        char *date_run[]={"./date",NULL};
        execv(date_run[0],date_run);
        exit(0);
    }
    return 0;
}

int main(){
    int i=1;
    char cur_direc[200]="Desktop\0";
    char shell_text[200]="shell ~/Desktop >> ";
    while(i==1){
        printf("%s",shell_text);
        char str[100];
        scanf("%s",str);
        if (strcmp("exit",str)==0){
            exit(0);
        }else if(strcmp("word",str)==0){
            char file_or_option1[100];
            scanf("%s",file_or_option1);
            char filename1[100];
            if(strcmp("-d",file_or_option1)==0 || strcmp("-n",file_or_option1)==0){
                char filename2[100];
                scanf("%s",filename1);
                if(strcmp("-d",file_or_option1)==0 ){
                    scanf("%s",filename2);
                    word(1,0,filename1,filename2,cur_direc);
                }else{
                    word(0,1,filename1,filename2,cur_direc);
                }
            }else{
                // if(file_or_option1[0]=='-'){
                //     printf("Wrong command\n");
                //     continue;
                // }
                word(0,0,file_or_option1,filename1,cur_direc);
            }
        }else if(strcmp("dir",str)==0){
            char file_or_option1[100];
            char file_or_option2[100];
            char dir_name[100];
            int r=0;
            int v=0;
            scanf("%s",file_or_option1);
            if(strcmp("-r",file_or_option1)==0 || strcmp("-v",file_or_option1)==0){
                scanf("%s",file_or_option2);
                if(strcmp("-r",file_or_option2)==0 || strcmp("-v",file_or_option2)==0){
                    scanf("%s",dir_name);
                }
            }
            int no_option=0;
            // printf("%s,%s,%s\n",file_or_option1,file_or_option2,dir_name);
            if(strcmp(file_or_option1,"-r")==0){
                r=1;
            }else if(strcmp(file_or_option1,"-v")==0){
                v=1;
            }else{
                no_option=1;
                dir(r,v,file_or_option1,cur_direc);
            }
            if(no_option==0){
                if(strcmp(file_or_option2,"-r")==0){
                    r=1;
                    dir(r,v,dir_name,cur_direc);
                }else if(strcmp(file_or_option2,"-v")==0){
                    v=1;
                    dir(r,v,dir_name,cur_direc);
                }else{
                    dir(r,v,file_or_option2,cur_direc);
                }
            }
        }else if(strcmp("date",str)==0){
            char file_or_option1[100];
            char file_or_option2[100];
            char filename[100];
            int R=0;
            int d=0;
            scanf("%s",file_or_option1);
            if(strcmp("-R",file_or_option1)==0 || strcmp("-d",file_or_option1)==0){
                scanf("%s",file_or_option2);
                if(strcmp("-R",file_or_option2)==0 || strcmp("-d",file_or_option2)==0){
                    scanf("%s",filename);
                }
            }
            int no_option=0;
            // printf("%s,%s,%s\n",file_or_option1,file_or_option2,dir_name);
            if(strcmp(file_or_option1,"-R")==0){
                R=1;
            }else if(strcmp(file_or_option1,"-d")==0){
                d=1;
            }else{
                no_option=1;
                date(R,d,file_or_option1,cur_direc);
            }
            if(no_option==0){
                if(strcmp(file_or_option2,"-R")==0){
                    R=1;
                    date(R,d,filename,cur_direc);
                }else if(strcmp(file_or_option2,"-d")==0){
                    d=1;
                    date(R,d,filename,cur_direc);
                }else{
                    date(R,d,file_or_option2,cur_direc);
                }
            }
        }
    }
    return 0;
}
