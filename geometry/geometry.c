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