#ifndef __MODEL_H__
#define __MODEL_H__


#include "../common.h"
#include "../geometry/geometry.h"
#define MAXLINE 100000

typedef struct Poly{
    int *vertices;
    int *texture_vertices;
    int *normal_vertices;
    size_t n_texture_vertices;
    size_t n_vertices;
    size_t n_normal_vertices;
} face;
 
typedef struct Model {
    v3d *vertices;
    v3d *texture_vertices;
    v3d *normal_vertices;
    face **faces;
    int n_faces;
    int n_vertices;
    int n_texture_vertices;
    int n_normal_vertices;
} model;

int read_obj (FILE *, model *);
void print_faces (model *);
void print_face (model *, size_t);
void print_vert (model *);
void print_text_vert (model *);
void norm_scale (model *);
double* read_v3d_from_obj_line (char *, char *);
face* read_poly_from_obj_line (face *, char *, char *);

#endif
