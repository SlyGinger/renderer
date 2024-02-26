#include "model.h"

double custom_atof(const char *str){
    int ret = 0;
    int ret1 = 0;
    double s = 1;
    int i = 0;
    int pwr = 1;
    
    while (str[++i] != '.' && str[i] && i < MAXLINE){
        ret *= 10;
        ret += str[i] - '0';
        printf ("test atof1 %d vs %c\n",ret, str[i]);
    } 
    if (str[i] == '.')
        while (str[++i]){
            pwr *= 10;
            ret1 = ret1 *10 + str[i]-'0';
            printf ("test atof2  %d vs %c\n",ret1, str[i]);
        }
    s = s*100000 + ret1 - 100000;
    s /= 10000000; 
    //s = s+1/ret; f
    if (str[0] == '-')
        return s;
    return ret1;
    
}

int  read_obj (FILE * f, model * mdl){
    
    char line[MAXLINE];
    char buff[MAXLINE];
    
    
    size_t pv = 0;
    size_t p = 2;
    size_t pp = 2;
    size_t cnt = 0;

    size_t v = 0;
    size_t vt = 0;
    size_t vn = 0;

    size_t v_cnt = 0;
    size_t vt_cnt = 0;
    size_t vn_cnt = 0;
    v3d vetrex; 

    //v3d *vertices = NULL;


    //vertices = (v3d *) malloc (sizeof(v3d));
    mdl->vertices = NULL;
    mdl->faces = NULL;
    mdl->n_vertices = 0;
    mdl->n_faces = 0 ;
    size_t line_pointer = 1;
    size_t buff_pointer = 0;
    size_t vertex_pointer = 0;
    size_t vertices_cnt = 0;
    uint8_t c;
    while (fgets(line,MAXLINE-1,f)){
        pv = 0;
        p = pp = 2;
        for (int i = 0; i < 3; i++){
                vetrex [i] = 0;
            }
            //verts
        if (!strncmp(line,"v ",2)){
            //printf ("read vert: %s", line);
            buff_pointer = 0;
            line_pointer = 1;
            size_t vertex_pointer = 0;
            while (line[line_pointer++]){
                if (isdigit(line[line_pointer]) || line[line_pointer] == '-' || line[line_pointer] == '.')
                    buff[buff_pointer++] = line[line_pointer];
                if (line[line_pointer] == ' ' || line[line_pointer] == '\n'){
                    //printf ("newbuff %s\n", buff);
                    vetrex[vertex_pointer++] = atof(buff);
                    memset(buff,'\0',buff_pointer);
                    buff_pointer = 0;
                }
            }
            vertices_cnt++;
            if (vertex_pointer == 2)
                vertex_pointer = 0;
            mdl->vertices = (v3d *) realloc (mdl->vertices ,sizeof(v3d) * vertices_cnt);
            for (int i = 0; i < 3; i++){
                mdl->vertices[vertices_cnt-1][i] = vetrex[i]; 
                //printf ("%lf ",vetrex[i]);
            }
            
        
        }
        //_________________poly handling___________________
        if (!strncmp (line,"f ", 2)){
           // printf ("read poly: %s", line);

            face * poly = NULL;
            poly = (face *) malloc (sizeof(face));
            poly->vertices = NULL;
            poly->nvertices = 0;
            v_cnt = 0;
            uint8_t space_pnt = 1;
            uint8_t slash_pnt = 1;
            uint8_t common_pnt = 1;
            uint8_t buff_pnt = 0;
            size_t buff_len =0;

            while (line[common_pnt++] ){


                //read till space
                while ((buff[buff_pnt++] = line[common_pnt]) != ' ' && (line[common_pnt++] != '\n'))
                ;
                buff[--buff_pnt] = '\0';
                buff_pnt = 0;
                //printf ("loop1 %s\n", buff);

                //take first int for atoi
                while (buff[buff_pnt++] != '/' && buff[buff_pnt++])
                ;
                buff[--buff_pnt] = '\0';
                //printf ("loop2 %s\n", buff);
                poly->nvertices++;
                poly->vertices = (int *) realloc(poly->vertices, sizeof(int) * poly->nvertices);
                poly->vertices[poly->nvertices-1] = atoi (buff)-1;
                buff_pnt = 0;
            }
                mdl->n_faces++;
                mdl->faces = (face **) realloc (mdl->faces, sizeof(face *) * mdl->n_faces);
                // mdl->faces[mdl->n_faces-1] = (face *) realloc (mdl->faces, sizeof(face) * mdl->n_faces);
                mdl->faces[mdl->n_faces-1] = poly;
                //printf ("vrt:%d\n",mdl->n_faces);
        }
    }
    mdl->n_vertices = vertices_cnt;
    return mdl->n_faces;
}

void print_vert (model *mdl){
    for (int i = 0; i < mdl->n_vertices; i++){
        printf ("vert#%d: %lf %lf %lf\n", i, mdl->vertices[i][0],mdl->vertices[i][1],mdl->vertices[i][2]);
    }
}
void print_faces (model * mdl){
    for (int i = 0; i < mdl->n_faces; i++){
        for (int j = 0; j < mdl->faces[i]->nvertices; j++){
            //if (mdl->faces[i]->nvertices != 3 )
            
                    printf("poly#%d: v#%d v index:%d x=%lf y=%lf z=%lf verts: %zu\n", i, j, mdl->faces[i]->vertices[j],
                        mdl->vertices[mdl->faces[i]->vertices[j]][0],
                        mdl->vertices[mdl->faces[i]->vertices[j]][1],
                        mdl->vertices[mdl->faces[i]->vertices[j]][2],
                        mdl->faces[i]->nvertices);
        }
    }
}

v3d *get_vertex (model *mdl, int poly, int vert ){
    return &mdl->vertices[mdl->faces[poly]->vertices[vert]];
}
int add_vertex (model * mdl, ...){

}

void print_face (model * mdl, size_t n){
    for (int j =0; j < mdl->faces[n]->nvertices; j++)
    printf("------poly#%zu: v#%d v index:%d x=%lf y=%lf z=%lf verts: %zu\n", n, j, mdl->faces[n]->vertices[j],
                        mdl->vertices[mdl->faces[n]->vertices[j]][0],
                        mdl->vertices[mdl->faces[n]->vertices[j]][1],
                        mdl->vertices[mdl->faces[n]->vertices[j]][2],
                        mdl->faces[n]->nvertices);
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