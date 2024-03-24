#include "model.h"

int  read_obj (FILE * f, model * mdl){
    
    char line[MAXLINE];
    double *vertex;
    mdl->vertices = NULL;
    mdl->faces = NULL;
    mdl->texture_vertices = NULL;
    mdl->normal_vertices = NULL;
    mdl->n_vertices = 0;
    mdl->n_normal_vertices  = 0;
    mdl->n_texture_vertices = 0;
    mdl->n_faces = 0 ;
    mdl->n_faces = 0 ;
    size_t line_pointer = 1;
    size_t buff_pointer = 0;
    size_t vertex_pointer = 0;
    size_t vertices_cnt = 0;
    size_t texture_vertices_cnt = 0;
    size_t normal_vertices_cnt = 0;
    while (fgets(line,MAXLINE-1,f)){
        if (!strncmp(line,"v ",2)){
            vertex =  read_v3d_from_obj_line (line,"v ");
            vertices_cnt++;
            mdl->vertices = (v3d *) realloc (mdl->vertices ,sizeof(v3d) * vertices_cnt);
            for (int i = 0; i < 3; i++){
                mdl->vertices[vertices_cnt-1][i] = vertex[i]; 
            }
            //free(vertex);
        }
        if (!strncmp(line,"vt  ",4)){
            vertex =  read_v3d_from_obj_line (line,"vt  ");            
            texture_vertices_cnt++;
            mdl->texture_vertices = (v3d *) realloc (mdl->texture_vertices ,sizeof(v3d) * texture_vertices_cnt);
            for (int i = 0; i < 3; i++){
                mdl->texture_vertices[texture_vertices_cnt-1][i] = vertex[i]; 
            }
            //free(vertex);
        }
        if (!strncmp(line,"vn  ",4)){
            vertex =  read_v3d_from_obj_line (line,"vn  ");            
            normal_vertices_cnt++;
                mdl->normal_vertices = (v3d *) realloc (mdl->normal_vertices ,sizeof(v3d) * normal_vertices_cnt);
            for (int i = 0; i < 3; i++){
                mdl->normal_vertices[normal_vertices_cnt-1][i] = vertex[i]; 
            }
            //free(vertex);
        }
        if (!strncmp (line,"f ", 2)){
            face * poly = NULL;
            poly = (face *) malloc (sizeof(face));
            poly->vertices = NULL;
            poly->texture_vertices = NULL;
            poly->normal_vertices = NULL;
            poly->n_normal_vertices = 0;
            poly->n_vertices = 0;
            poly->n_texture_vertices = 0;
            read_poly_from_obj_line(poly, line, "f ");
            mdl->n_faces++;
            mdl->faces = (face **) realloc (mdl->faces, sizeof(face *) * mdl->n_faces);
            mdl->faces[mdl->n_faces-1] = poly;
        }
    }
    mdl->n_vertices = vertices_cnt;
    mdl->n_texture_vertices = texture_vertices_cnt;
    mdl->n_normal_vertices = normal_vertices_cnt;
    return mdl->n_faces;
}

void print_vert (model *mdl){
    for (int i = 0; i < mdl->n_vertices; i++){
        printf ("vert#%d: %lf %lf %lf\n", i, mdl->vertices[i][0],mdl->vertices[i][1],mdl->vertices[i][2]);
    }
}

void print_text_vert (model *mdl){
    for (int i = 0; i < mdl->n_normal_vertices; i++){
        printf ("text_normal#%d: %lf %lf %lf\n", i, mdl->normal_vertices[i][0],mdl->normal_vertices[i][1],mdl->normal_vertices[i][2]);
    }
}

void print_faces (model * mdl){
    for (int i = 0; i < mdl->n_faces; i++){
        for (int j = 0; j < mdl->faces[i]->n_vertices; j++){
            //if (mdl->faces[i]->n_vertices != 3 )
            
                    printf("poly#%d: point#%d  vertex#:%d x=%lf y=%lf z=%lf verts: %zu\n", i, j, mdl->faces[i]->vertices[j],
                        mdl->vertices[mdl->faces[i]->vertices[j]][0],
                        mdl->vertices[mdl->faces[i]->vertices[j]][1],
                        mdl->vertices[mdl->faces[i]->vertices[j]][2],
                        mdl->faces[i]->n_vertices);
                    printf("poly#%d: t_point#%d t_vertex:%d x=%lf y=%lf z=%lf verts: %zu\n", i, j, mdl->faces[i]->texture_vertices[j],
                        mdl->texture_vertices[mdl->faces[i]->texture_vertices[j]][0],
                        mdl->texture_vertices[mdl->faces[i]->texture_vertices[j]][1],
                        mdl->texture_vertices[mdl->faces[i]->texture_vertices[j]][2],
                        mdl->faces[i]->n_texture_vertices);
        }
    }
}


void print_face (model * mdl, size_t n){
    for (int j =0; j < mdl->faces[n]->n_vertices; j++)
    printf("------poly#%zu: v#%d v index:%d x=%lf y=%lf z=%lf verts: %zu\n", n, j, mdl->faces[n]->vertices[j],
                        mdl->vertices[mdl->faces[n]->vertices[j]][0],
                        mdl->vertices[mdl->faces[n]->vertices[j]][1],
                        mdl->vertices[mdl->faces[n]->vertices[j]][2],
                        mdl->faces[n]->n_vertices);
}
void norm_scale (model *mdl){
    //printf ("lolV: %d", mdl->n_vertices);
    double max = mdl->vertices[0][0];
    int i = 0;
    int j = 0;
    int i_max = -1;
        for (i = 0; i < mdl->n_vertices; i++){
            for (j = 0; j < 3; j++){
                if (fabs(mdl->vertices[i][j])> max){
                    max = fabs(mdl->vertices[i][j]);
                    i_max = i;
                    }
            }
    }
    //printf ("maxvert: %d %d!",i_max, j );
    for (int i = 0; i < mdl->n_vertices; i++){
        for (int j = 0; j < 3; j++){
                //printf ("%lf was norm to", mdl->vertices[i][j]);
                mdl->vertices[i][j] /= max;
                //printf (" %lf\n", mdl->vertices[i][j]);
        }
    }
}

double* read_v3d_from_obj_line (char * line, char *str){
    char buff[MAXLINE];
    double *point;
    int i = 0;
    int j = 0;
    int k = 0;
    point = (double*) malloc (sizeof(double)*3);

    if (!strncmp(line,str,i = strlen(str))){
        for (k = 0; k < 3; k++){
            while ((buff[j++] = line[i++]) != ' ')
            ;
            point[k] = atof(buff);

            j = 0;
        }
    }
    return point;
}

face * read_poly_from_obj_line (face * poly, char * line, char * str){
    char buff[MAXLINE];
    int arr[3];
    int i = 0;
    int j = 0;
    int k = 0;
    if (!strncmp(line,str,i = strlen(str))){
        while (line[i] != '\n' && line[i]){
            while ((buff[j++] = line[i++]) != '/' && line[i-1] != ' ')
            ;
            arr[k++] = atoi (buff)-1;
            j = 0;
            if (line[i-1] == ' '){
                //printf ("testface k=%d v=%d vt=%d vn = %d\n",k,arr[0],arr[1],arr[2]);
                poly->n_vertices++;
                poly->n_texture_vertices++;
                poly->n_normal_vertices++;
                poly->vertices = (int *) realloc(poly->vertices, sizeof(int) * poly->n_vertices);
                poly->vertices[poly->n_vertices-1] = arr[0];
                poly->texture_vertices = (int *) realloc(poly->texture_vertices, sizeof(int) * poly->n_texture_vertices);
                poly->texture_vertices[poly->n_texture_vertices-1] = arr[1];
                poly->normal_vertices = (int *) realloc(poly->normal_vertices, sizeof(int) * poly->n_normal_vertices);
                poly->normal_vertices[poly->n_normal_vertices-1] = arr[2];

                k = 0;
            }
            
        }
        return poly;
    }
    return NULL;
}

int load_diffuse_texture (model * mdl, char * filename){
    tga_image * img;
    img = read_tga (filename);
    mdl->diffuse_texture = NULL;
    mdl->diffuse_height = img->header->height;
    mdl->diffuse_width= img->header->width;
    mdl->diffuse_texture = free_tga(img);
    if (mdl->diffuse_texture)
        return 1;
    return 0;

}