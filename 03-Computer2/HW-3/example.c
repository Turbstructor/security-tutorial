#include<stdio.h>
int main()
{
    int a = 2;
    a = a * 2;

    int b = 2;
    b = b << 1;

    int c = 1;
    c = c * 1024;

    int d = 1;
    d = d << 10;

    int e = 4;
    e = e / 2;

    int f = 4;
    f = f >> 1;

    int g = 1024;
    g = g / 1024;

    int h = 1024;
    h = h >> 10;

    int i = 3;
    i = i % 2;

    int j = 3;
    j = j & 1;

    return 0;
}
