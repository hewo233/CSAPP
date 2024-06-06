#include <stdio.h>

int main()
{
    int a, b ,c;
    a = b = c = 1;
    printf("%d %d %d %d", a, b, c, (a && b && c));
    return 0;
}