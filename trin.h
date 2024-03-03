#ifndef __TRIN_H__
#define __TRIN_H__
#include "model/model.h"
#include "tga/tga.h"
#include "geometry/geometry.h"


int triangle (tga_image *, v3i , v3i, v3i, double);
//int triangle_from_z (tga_image *, v3i , v3i, v3i, double);
int triangle_face (tga_image *, model *, face *, int *);

int get_line_i_x (v2i, v2i, int);
int get_line_i_y (v2i, v2i, int);
void print_v2i (v2i);


#endif