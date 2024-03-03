#ifndef __MODEL_H__
#define __MODEL_H__


#include "../common.h"
#include "../geometry/geometry.h"
#define MAXLINE 1000

typedef struct Poly{
    int *vertices;
    size_t nvertices;
} face;
 
typedef struct Model {
    v3d *vertices;
    face **faces;
    int n_faces;
    int n_vertices;
} model;

int read_obj (FILE *, model *);
void print_faces (model *);
void print_face (model *, size_t);
void print_vert (model *);
void norm_scale (model *);

#endif
