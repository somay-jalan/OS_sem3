#include<stdio.h>
#include<math.h>

int main(){
    double a=1;
    printf("Counting started at %f\n",a);
    while(a<pow(2,32)){
        a++;
    }
    printf("Counting ends at %f\n",a);
}
