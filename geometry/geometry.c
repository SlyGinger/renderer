#include "geometry.h"

double dot_prod(v3d a, v3d b){
    return a[0]*b[0]+a[1]*b[1]+a[2]*b[2];
}

v3d * cross_prod (v3d d, v3d a, v3d b){
    v3d c;
    c[1] = a[1]*b[2] - a[2]*b[1];
    c[2] = a[2]*b[0] - a[0]*b[2];
    c[0] = a[0]*b[1] - a[1]*b[0];
    for (int i = 0; i < 3; i++){
        d[i] = c[i];
    }
    return d;
}

v3d * norm_v3d (v3d a){
    double len = sqrt(a[0]*a[0]+a[1]*a[1]+a[2]*a[2]);
    for  (int i = 0; i < 3; i++)
        a[i] /= len;
    return a;
}