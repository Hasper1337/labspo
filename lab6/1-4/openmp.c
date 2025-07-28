#include <stdio.h>
#include <math.h>
#include <omp.h>

long double f(long double x) {
    return (x <= 0) ? powl(x, 2)*sinl(cbrtl(x)-3) : sqrtl(x)*cosl(2*x);
}

long double integrate(long double a, long double b, long double eps) {
    long double m = (a+b)/2, fm = f(m);
    long double S = (f(a)+4*fm+f(b))*(b-a)/6;
    long double SL = (f(a)+4*f((a+m)/2)+fm)*(m-a)/6;
    long double SR = (fm+4*f((m+b)/2)+f(b))*(b-m)/6;
    return fabsl(SL+SR-S) < eps ? SL+SR : integrate(a,m,eps/2)+integrate(m,b,eps/2);
}

int main() {
    long double r1,r2;
    #pragma omp parallel sections
    {
        #pragma omp section
        r1 = integrate(-2,0,1e-15);
        #pragma omp section
        r2 = integrate(0,1,1e-15);
    }
    printf("Result: %.20Lf\n", r1+r2);
    return 0;
}
