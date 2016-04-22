#include<stdio.h>
#include"fib.h"
#include"add.h"
int main()
{
    int n=0;
    printf("input n=\n");
    scanf("%d",&n);
    printf("fib(%d=%d\n",n,fib(n));
    printf("add:%d\n",add());
    return 0;
}
