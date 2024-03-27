#include "tga/tga.h"
#include "model/model.h"
#include "trin.h"


int main (void){
    tga_image tga;
    tga_image *texture;
    
    //african_head_diffuse.tga
    tga = *create_tga(1000,1000);
    //texture = create_tga (20)
    model m;
    FILE * f;
    //print_metadata(tga.header);
    f = fopen("a.obj","r");
    read_obj(f,&m);
    norm_scale(&m);
    int * z_buff;
    z_buff = (int *) malloc (sizeof (int) * tga.header->width * tga.header->height);
    for (int i = 0; i< tga.header->width * tga.header->height; i++)
        z_buff[i] = INT_MIN;
    //print_faces(&m);
    printf ("faces\n%d",m.n_faces);
    load_diffuse_texture(&m,"chech.tga");
    printf ("h:%d w:%d\n", m.diffuse_height,m.diffuse_width);
    for (int i = 0; i < m.n_faces; i++){
        v2i t0;
        v2i t1;
        v2i t2; 
        int line_cnt = 0;
        triangle_face(&tga, &m,m.faces[i],z_buff);
    }
    
    dump_z_buff(&tga,z_buff);
    // v2i a = {50,40};
    // v2i b = {20,70};
    // v2i c = {50,15};
    // triangle(&tga,a,b,c);
    //triangle_face (&tga, &m,m.faces[5]);
    write_tga("z1.tga", &tga);
   //texture = read_tga ("head_diffuse.tga");
   // write_tga("chech.tga", texture);
    //print_faces(&m);
    //print_face(&m,1);
    //print_vert(&m);
    //print_text_vert(&m);
    
    
   
    

    
   // printf ("y=%d x=%d",  get_line_i_y(a,b,11), get_line_i_x(a,b,38));
}

