#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__
#include "../common.h"
#define Z_DEPTH 1024
typedef double v3d[3];
typedef int v2i [2];
typedef int v3i [3];

void * v3d_norm (v3d);
double v3d_dot_prod (v3d,v3d);
void * v3d_add (v3d,v3d,v3d);
void * v3d_sub (v3d,v3d,v3d);
void * v3d_mul (v3d,double);

void * v3i_add (v3i,v3i,v3i,int);
void * v3i_sub (v3i,v3i,v3i);
void * v3i_mul (v3i,double);

void print_v3i (v3i);
void print_v3d (v3d);

void * v3d_cross_prod (v3d,v3d,v3d);
void v3d_set (v3d,v3d);
void v3i_set (v3i,v3i);
void v3d_swap (v3d, v3d);
void v3i_swap (v3i, v3i);





#endif