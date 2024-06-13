#include <threads.h> // 包含 CSAPP 相关的库和函数定义

void *thread(void *vargp); /* 线程函数原型 */

/* 全局共享变量 */
volatile long cnt = 0; /* 计数器 */

int main(int argc, char **argv) {
    long niters;
    pthread_t tid1, tid2; // 定义两个线程的 ID

    /* 检查输入参数 */
    if (argc != 2) {
        printf("usage: %s <niters>\n", argv[0]);
        exit(0);
    }
    niters = atoi(argv[1]); // 将命令行参数转换为整数，即迭代次数

    /* 创建线程并等待它们完成 */
    Pthread_create(&tid1, NULL, thread, &niters);
    Pthread_create(&tid2, NULL, thread, &niters);
    Pthread_join(tid1, NULL);
    Pthread_join(tid2, NULL);

    /* 检查结果 */
    if (cnt != (2 * niters))
        printf("BOOM! cnt=%ld\n", cnt);
    else
        printf("OK cnt=%ld\n", cnt);
    exit(0);
}

/* 线程函数 */
void *thread(void *vargp) {
    long i, niters = *((long *)vargp);

    for (i = 0; i < niters; i++)
        cnt++; // 累加全局变量

    return NULL;
}
