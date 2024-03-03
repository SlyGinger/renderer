#include "trin.h"

int triangle (tga_image * img, v2i p0, v2i p1, v2i p2, double intensity){
    //sort
    //print_metadata(img->header);
    tga_color color;
    tga_color color2;
    set_color (&color,RAW, 255*intensity,255*intensity,255*intensity,0);
    set_color (&color2,RAW, 255,255,255,0);
    v2i t0 = {p0[0],p0[1]};
    v2i t1 = {p1[0],p1[1]};
    v2i t2 = {p2[0],p2[1]}; 
    
    if (t0[1] > t1[1]) {
        swap_int(&t0[1],&t1[1]);
        swap_int(&t0[0],&t1[0]);
    }
    if (t0[1] > t2[1]) {
        swap_int(&t0[1],&t2[1]);
        swap_int(&t0[0],&t2[0]);
    }
    if (t1[1] > t2[1]) {
        swap_int(&t1[1],&t2[1]);
        swap_int(&t1[0],&t2[0]);
    }
    //up
    int xi = 0;
    int x0 = t1[0];
    for (int yi = t1[1]; yi < t2[1]; yi++){
        //int xn = get_line_i_x(t0, t2, yi);
        int xn = t0[0] == t2[0] ? t0[0] : get_line_i_x(t0, t2, yi);
        x0 = get_line_i_x(t1, t2, yi);
        x0 = t1[0] == t2[0] ? t1[0] : get_line_i_x(t1, t2, yi);
        //printf ("up new line x0=%d x1=%d y=%d\n",x0,xn,yi);
        if (xn > x0){
            xi = x0;
        } else {
            xi = xn;
            xn = x0;
        }
        for (; xi <= xn; xi++){
            set_pixel(img, xi, yi, &color);
        }
    }
    //down
   
    xi = 0;
    x0 = t1[0];
   // printf ("\tstart t0=%d t1=%d t2=%d\n", t0[1],t1[1],t2[1]);
    for (int yi = t1[1]; yi > t0[1]; yi--){
      //  printf ("down\n");
        //printf ("\t down new line yi = %d t0=%d t1=%d t2=%d\n",yi, t0[1],t1[1],t2[1]);
        int xn = t0[0] == t2[0] ? t0[0] : get_line_i_x(t0, t2, yi);
        x0 = t0[0] == t1[0] ? t0[0] : get_line_i_x(t1, t0, yi);
        //printf ("XN:%d\n",xn);
        //printf ("X0:%d\n",x0);

       // printf ("\tstart t0=%d t1=%d t2=%d\n", t0[1],t1[1],t2[1]);
        //printf ("\ttri x0=%d xn=%d y=%d -> t0=%d\n",x0,xn,yi, t0[1]);
        if (xn > x0){
            xi = x0;
        } else {
            xi = xn;
            xn = x0;
        }
        for (; xi <= xn; xi++){
            set_pixel(img, xi, yi, &color);
        }
    }

    //line(img,t0[0],t0[1],t1[0],t1[1]);
    //line(img,t1[0],t1[1],t2[0],t2[1]);
    //line(img,t0[0],t0[1],t2[0],t2[1]);
    return 0;
}

int triangle_face  (tga_image *img, model * mdl, face  * poly){
    v2i t0, t1, t2;
    //int x0 = (m.vertices[m.faces[i]->vertices[j]][0]+1.) * tga.header->width/2;

    //screen_coords
    // t0[0] =  (mdl->vertices[poly->vertices[0]][0]+1) * img->header->width/2;
    // t0[1] =  (mdl->vertices[poly->vertices[0]][1]+1) * img->header->height/2;
    // t1[0] =  (mdl->vertices[poly->vertices[1]][0]+1) * img->header->width/2;
    // t1[1] =  (mdl->vertices[poly->vertices[1]][1]+1) * img->header->height/2;
    // t2[0] =  (mdl->vertices[poly->vertices[2]][0]+1) * img->header->width/2;
    // t2[1] =  (mdl->vertices[poly->vertices[2]][1]+1) * img->header->height/2;
    //world coords
    v2i screen_coords[3];
    v3d world_coords[3];
    v3d ab;
    v3d bc;
    for (int i = 0; i < 3; i++){
        screen_coords[i][0] = (mdl->vertices[poly->vertices[i]][0]+1) * img->header->width/2;
        screen_coords[i][1] = (mdl->vertices[poly->vertices[i]][1]+1) * img->header->height/2;
        world_coords[i][0] = mdl->vertices[poly->vertices[i]][0]+1;
        world_coords[i][1] = mdl->vertices[poly->vertices[i]][1]+1;
        world_coords[i][2] = mdl->vertices[poly->vertices[i]][2]+1;
        
    }
    for (int i = 0; i < 3; i++){
        ab[i] = world_coords[2][i] - world_coords[0][i];
        bc[i] = world_coords[1][i] - world_coords[0][i];
    }
    cross_prod(ab,ab,bc);
    norm_v3d(ab);
    //norm = ab
    //(world_coords[2]-world_coords[0])^(world_coords[1]-world_coords[0]);
    v3d v_light = {-1,0,0};
    double intensity = dot_prod(v_light,ab);
    printf ("int = %lf\n", intensity);


    // printf("!!!%d\n", t2[0]);
    // print_v2i (t0);
    // print_v2i (t1);
    // print_v2i (t2);
    if (intensity > 0)
        triangle(img,screen_coords[0],screen_coords[1],screen_coords[2],intensity);
    return 0;
}


int get_line_i_x(v2i a, v2i b, int y){
    double k = 0;
    v2i t0 = {a[0],a[1]};
    v2i t1 = {b[0],b[1]};
    if (t0[0] < t1[0]){
        swap_int (&t0[0],&t1[0]);
        swap_int (&t0[1],&t1[1]);
    }
    
    k =  ((double) (t0[1] - t1[1])) / (double) (t0[0] - t1[0]);

    double c = t0[1] - k * t0[0];
    //printf ("!linear function: %d = %lf * x + %lf (%lf %lf)\n",y,k,c,(double) (t0[1] - t1[1]),(double) (t0[0] - t1[0]));
    //printf ("!linear function x%lf\n", (y - c)/k);
    
    return (y - c)/k;
}

int get_line_i_y (v2i a, v2i b, int x){
    double k = 0;
    int c = a[1] > b[1] ? b[1] : a[1];
    k =  ((double) (a[1] - b[1])) / (double) (a[0] - b[0]);
    printf ("k=%lf c=%d\n",k,c);
    return x*k+c;
}

void print_v2i (v2i a){
    printf ("v2i x:%d y:%d\n", a[0],a[1]);
}