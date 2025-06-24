#include <stdio.h>
#include <stdlib.h>

double nn(double v, double eps_x, double eps_v);

int main(int argc, char **argv)
{
    if(argc < 4)
        return 0;

    double x = nn(atof(argv[1]),atof(argv[2]),atof(argv[3]));
    printf("x = %f\n", x);
    return 0;
}
