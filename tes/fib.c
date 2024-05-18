int fib(int n)
{
    int x, y;
    x = 0, y = 1;
    while (n > 0)
    {
        y = x + y;
        x = y - x;
        n = n - 1;
    }
    return y;
}