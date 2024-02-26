#ifndef __MODEL_H__
#define __MODEL_H__


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#define MAXLINE 1000



typedef double v3d[3];
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

int add_vetex (model *, v3d);
int add_face (model *, ...);

int read_obj (FILE *, model *);
v3d *get_vertex (model *, int, int);
void print_faces (model *);
void print_face (model *, size_t);
void print_vert (model *);
void norm_scale (model *);

double custom_atof (const char *);

#endif
