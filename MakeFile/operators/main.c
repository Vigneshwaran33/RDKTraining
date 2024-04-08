#include<stdio.h>
#include "mheaders.h"

int num1=20;
int num2;

static int num3=10;
static int num4;

int num5=5;



int main(){
num2=10;
num4=5;
static int num6=10;
printf("Addition : %d\n",add(num1,num2));
printf("Subtraction : %d\n",sub(num3,num4));
printf("IsEqual %d\n",isEqual(num5,num6));

printf("num1 %p",&num1);

}
