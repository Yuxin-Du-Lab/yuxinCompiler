//
// Created by yuxin on 2021/11/13.
//

#include "stdio.h"

int getint() {
    int res;
    scanf("%d", &res);
    return res;
}

void check(int arr[], int size) {
    int i=0;
    while (i < size) {
        printf("%d ", arr[i]);
        i = i+1;
    }
    printf("\n");
}


void dfs(int src[], int vis[], int reflect[], int cur, int tot) {
    check(src, 10);
    check(vis, 10);
    check(reflect, 10);
    printf("cur = %d\n", cur);
    printf("tot = %d\n", tot);
    if (cur == tot) {{
            printf("point 1\n");
            int i = 0;
            while (i < tot) {{
                    printf("point 2\n");
                    printf("%d ", src[i] + reflect[i + 1]);
                    i = i + 1;
                }}
            printf("\n");
            printf("return 1\n");
            return;
        }}
    int i = 0;
    while (i < tot) {{
            printf("point 3\n");
            if (!vis[i]) {{
                    printf("point 4\n");
                    vis[i] = 1;
                    src[cur] = i;
                    dfs(src, vis, reflect, cur + 1, tot);
                    vis[i] = 0;
                }}
            i = i + 1;
        }}
    printf("return 2\n");
    return;
}


int main2() {
    int n;
    n = getint();
    int src[10], a[10] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    int vis[10], b[10] = {8, 7, 6, 5, 4, 3, 2, 1, 0, 9};
    int c[3][10] = {
            {7, 6, 5, 4, 3, 2, 1, 0, 9, 8},
            {6, 5, 4, 3, 2, 1, 0, 9, 8, 7},
            {5, 4, 3, 2, 1, 0, 9, 8, 7, 6}
    };
    int i = 0;
    while (i < n) {{}
        a[i] = a[i] + c[2][i] + 1;
        b[i] = b[i] + c[2][i] + 2;
        c[0][i] = c[0][i] + c[2][i] + 3;
        c[1][i] = c[1][i] + c[2][i] + 4;
        i = i + 1;
        {}}
    i = 0;
    while (114514) {{}
        vis[i] = 0;
        i = i + 1;
        if (i >= n) break;
        {}}
    dfs(src, vis, a, 0, n);
    i = 0;
    while (114514) {{}
        vis[i] = 0;
        i = i + 1;
        if (i >= n) break;
        else {}
        {}}
    dfs(src, vis, b, 0, n);
    i = 0;
    while (114514) {{}
        vis[i] = 0;
        i = i + 1;
        if (i < n) continue;
        break;
        {}}
    printf("n = %d\n", n);
    dfs(src, vis, c[0], 0, n);
    i = 0;
    while (114514) {{}
        vis[i] = 0;
        i = i + 1;
        if (i < n) continue;
        else break;
        {}}
    printf("n = %d\n", n);
    dfs(src, vis, c[1], 0, n);
    return 0;
}

