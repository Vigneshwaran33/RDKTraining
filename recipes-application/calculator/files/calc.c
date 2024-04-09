#include<stdio.h>
#include<stdlib.h>

void add(int a,int b){ printf("Result:%d",a+b); }
void sub(int a,int b){ printf("Result:%d",a-b); }
void mul(int a,int b){ printf("Result:%d",a*b); }
void divide(int a,int b){ printf("Result:%d",a/b); }
int main()
{
        printf("1:Add\n2:Subtract\n3:Multiply\n4:Divide\n");
        int op,a,b;
        printf("Enter Operation number: ");
        scanf("%d",&op);
        printf("Enter the values of a and b: ");
        scanf("%d %d",&a,&b);
        if(op==1)add(a,b);
        else if(op==2)sub(a,b);
        else if(op==3)mul(a,b);
        else if(op==4)divide(a,b);
        else printf("Invalid Input");
}
