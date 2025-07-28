#include <stdio.h>
#include <math.h>
#include <pthread.h>

typedef struct { long double a,b,eps,res; } Args;

long double f(long double x) {
    return (x <= 0) ? (x*x)*sinl(cbrtl(x)-3) : sqrtl(x)*cosl(2*x);
}

long double serial_integrate(long double a, long double b, long double eps) {
    long double m = (a+b)/2, fm = f(m);
    long double S = (f(a)+4*fm+f(b))*(b-a)/6;
    long double SL = (f(a)+4*f((a+m)/2)+fm)*(m-a)/6;
    long double SR = (fm+4*f((m+b)/2)+f(b))*(b-m)/6;
    return fabsl(SL+SR-S) < eps ? SL+SR : 
           serial_integrate(a,m,eps/2)+serial_integrate(m,b,eps/2);
}

void* integrate(void* arg) {
    Args *a = (Args*)arg;
    a->res = serial_integrate(a->a, a->b, a->eps);
    return NULL;
}

int main() {
    Args a1 = {-2,0,1e-15}, a2 = {0,1,1e-15};
    pthread_t t1,t2;
    
    pthread_create(&t1, NULL, integrate, &a1);
    pthread_create(&t2, NULL, integrate, &a2);
    
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    
    printf("Result: %.20Lf\n", a1.res+a2.res);
    return 0;
}
