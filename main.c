#include "tga/tga.h"
#include "model/model.h"
#include "trin.h"


int main (void){
    tga_image tga;
    tga = *create_tga(1000,1000);
    model m;
    FILE * f;
    print_metadata(tga.header);
    f = fopen("t.obj","r");
    read_obj(f,&m);
    norm_scale(&m);
    int * z_buff;
    z_buff = (int *) malloc (sizeof (int) * tga.header->height * tga.header->width);
    //print_faces(&m);
    for (int i = 0; i < m.n_faces; i++){
        v2i t0;
        v2i t1;
        v2i t2; 
        int line_cnt = 0;
        //print_face(&m, i);
        // for (int j = 0; j < 3; j++){
                
        //         size_t nfaces = m.faces[i]->nvertices-1;
        //         int x0 = (m.vertices[m.faces[i]->vertices[j]][0]+1.) * tga.header->width/2;
        //         int y0 = (m.vertices[m.faces[i]->vertices[j]][1]+1.) * tga.header->height/2;
        //         int x1 = (m.vertices[m.faces[i]->vertices[(j+1)%nfaces]][0]+1.) * tga.header->width/2;
        //         int y1 = (m.vertices[m.faces[i]->vertices[(j+1)%nfaces]][1]+1.) * tga.header->height/2;

        //         if (j == 1){
        //             t0[0] = x0;
        //             t0[1] = y0;
        //         }
        //         if (j == 2){
        //             t1[0] = x0;
        //             t1[1] = y0;
        //         }
        //         if (j == 1){
        //             t1[0] = x0;
        //             t1[1] = y0;
        //         }
        //         //printf ("start line f:%d %d %d %d %d\n",i, x0,y0,x1,y1);
                
        //         line_cnt += line(&tga,x0,y0,x1,y1);
        // }
        // if (line_cnt < 3){
        //     printf("wring line at %d\n", i);
        // }
        triangle_face(&tga, &m,m.faces[i], z_buff);
    }
    // v2i a = {50,40};
    // v2i b = {20,70};
    // v2i c = {50,15};
    // triangle(&tga,a,b,c);
    //triangle_face (&tga, &m,m.faces[5]);
    write_tga("b1.tga", &tga);
    
   
    

    
   // printf ("y=%d x=%d",  get_line_i_y(a,b,11), get_line_i_x(a,b,38));
}

