#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__
#include "../common.h"

typedef double v3d[3];
typedef int v2i [2];
typedef int v3i [3];

void * v3d_norm (v3d);
double v3d_dot_prod (v3d,v3d);
void * v3d_add (v3d,v3d,v3d);
void * v3d_sub (v3d,v3d,v3d);
void * v3d_mul (v3d,double);
void * v3d_cross_prod (v3d,v3d,v3d);
void v3d_set_v3d (v3d,v3d);
void v3d_swap (v3d, v3d);





#endif