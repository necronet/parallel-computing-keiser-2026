#include <stdio.h>

int main(void)
{
    int n = 4;
    float a = 2.0f;
    float x[] = {1, 2, 3, 4};
    float y[] = {10, 20, 30, 40};

    for (int i = 0; i < n; i++) {
        y[i] = a * x[i] + y[i];
    }

    for (int i = 0; i < n; i++) {
        printf("%.0f ", y[i]);
    }
    printf("\n");

    return 0;
}
