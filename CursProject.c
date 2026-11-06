#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>
#include <math.h>

int analiz(const char* name);

int main(void)
{
    setlocale(LC_ALL, "RUS");

    analiz("RPC");
    analiz("NIC");
    analiz("HDD");

    return 0;
}

int analiz(const char* name)
{
    printf("йНЛОНМЕМР: %s\n", name);

    double x1, x2, x3, x4, x5;
    double q4 = 0.0, q5 = 0.0;

    printf("X[1] = ");
    scanf("%lf", &x1);
    printf("X[2] = ");
    scanf("%lf", &x2);
    printf("X[3] = ");
    scanf("%lf", &x3);
    printf("X[4] = ");
    scanf("%lf", &x4);
    printf("X[5] = ");
    scanf("%lf", &x5);

    double l_min = -1.5;
    double l_max = 20.0;


    if (x4 != 0.0 && x1 != 0.0) {
        q4 = log(x4 / x1);
        if (q4 < l_min || q4 > l_max)
            printf("Q[4] = %.2f рпебнцю\n", q4);
        else
            printf("Q[4] = %.2f мнплю\n", q4);
    }
    else {
        printf("Q[4] = опносяй (МЕР ДЮММШУ)\n");
    }

    if (x5 != 0.0 && x2 != 0.0) {
        q5 = log(x5 / x2);
        if (q5 < l_min || q5 > l_max)
            printf("Q[5] = %.2f рпебнцю\n", q5);
        else
            printf("Q[5] = %.2f мнплю\n", q5);
    }
    else {
        printf("Q[5] = опносяй (МЕР ДЮММШУ)\n");
    }

    printf("\n");
    return 0;
}