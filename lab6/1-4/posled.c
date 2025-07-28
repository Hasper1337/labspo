#include <stdio.h>
#include <math.h>

long double f(long double x) {
    return (x <= 0) ? (x*x)*sinl(cbrtl(x)-3) : sqrtl(x)*cosl(2*x);
}

long double integrate(long double a, long double b, long double eps) {
    long double mid = (a + b)/2, fmid = f(mid);
    long double S = (f(a) + 4*fmid + f(b))*(b-a)/6;
    long double SL = (f(a) + 4*f((a+mid)/2) + fmid)*(mid-a)/6;
    long double SR = (fmid + 4*f((mid+b)/2) + f(b))*(b-mid)/6;
    
    return fabsl(SL+SR - S) < eps ? SL+SR : integrate(a,mid,eps/2)+integrate(mid,b,eps/2);
}

int main() {
    printf("Result: %.20Lf\n", integrate(-2,0,1e-15)+integrate(0,1,1e-15));
    return 0;
}
