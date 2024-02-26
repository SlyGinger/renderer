
#include "model/model.h"
#include "tga/tga.h"
typedef int v2i [2];

int triangle (tga_image *, v2i , v2i, v2i, double);
int triangle_face (tga_image *, model *, face *);
int get_line_i_x (v2i, v2i, int);
int get_line_i_y (v2i, v2i, int);
void print_v2i (v2i);
v3d *norm_v3d (v3d);
double dot_prod (v3d,v3d);
v3d * cross_prod (v3d,v3d,v3d);
