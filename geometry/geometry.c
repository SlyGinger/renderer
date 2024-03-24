#include "geometry.h"

double v3d_dot_prod(v3d a, v3d b){
    return a[0]*b[0]+a[1]*b[1]+a[2]*b[2];
}

void * v3d_cross_prod (v3d d, v3d a, v3d b){
    v3d c;
    c[1] = a[1]*b[2] - a[2]*b[1];
    c[2] = a[2]*b[0] - a[0]*b[2];
    c[0] = a[0]*b[1] - a[1]*b[0];
    for (int i = 0; i < 3; i++){
        d[i] = c[i];
    }
    //return d;
}

void * v3d_norm (v3d a){
    double len = sqrt(a[0]*a[0]+a[1]*a[1]+a[2]*a[2]);
    for  (int i = 0; i < 3; i++)
        a[i] /= len;
    //return a;
}

void v3d_swap (v3d a, v3d b){
    double t;
    for  (int i = 0; i < 3; i++){
        t = a[i];
        a[i] = b[i];
        b[i] = t;
    }
}

void v3i_swap (v3i a, v3i b){
    int t;
    for  (int i = 0; i < 3; i++){
        t = a[i];
        a[i] = b[i];
        b[i] = t;
    }
}

void v3d_set (v3d a, v3d b){
    for (int i = 0; i < 3; i++)
        a[i] = b[i];
}

void v3i_set (v3i a, v3i b){
    for (int i = 0; i < 3; i++)
        a[i] = b[i];
}

void * v3i_add(v3i c, v3i a, v3i b, int sign){
    //sign = sign/abs(sign);
    if (sign > 0)
        for (int i = 0; i < 3; i++){
            //printf ("%d + %d",a[i],b[i]);
            c[i] = a[i] + b[i];
            //printf ("= %d\n",c[i]);
        }
    else 
        for (int i = 0; i < 3; i++){
            //printf ("%d - %d",a[i],b[i]);
            c[i] = a[i] - b[i];
            //printf ("= %d\n",c[i]);
        }
    return c;
}

void * v3i_mul(v3i a, double x){
    for (int i = 0; i < 3; i++){
        //printf ("%d * %lf",a[i],x);
        a[i] = (double)a[i] * x + 0.5;
        //printf ("=%d\n",a[i]);
    }
    return a;
}

void print_v3i (v3i a){
    printf ("x=%d y=%d z=%d\n",a[0],a[1],a[2]);
}

void print_v3d (v3d a){
    printf ("x=%lf y=%lf z=%lf\n",a[0],a[1],a[2]);
}