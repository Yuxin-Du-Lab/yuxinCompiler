//
// Created by yuxin on 2021/11/13.
//

#include "stdio.h"

int getint() {
    int res;
    scanf("%d", &res);
    return res;
}

int fibo(int n) {
    if (n==1 || n==2) {
        return 1;
    }
    return fibo(n-1) + fibo(n-2);
}

//int main() {
//    int n;
//    n = getint();
//    int res = fibo(n);
//    printf("%d\n", res);
//    return 0;
//}

