#include "trin.h"

int triangle (tga_image * img, v3i p0, v3i p1, v3i p2, double intensity, int * z_buff){
    //printf ("strating tring\n");
    tga_color color;
    tga_color color2;
    tga_color color3;
    set_color (&color,RAW, 255*intensity,255*intensity,255*intensity,0);
    set_color (&color2,RAW, 255,0,255,0);
    set_color (&color3,RAW, 0,255,255,0);
    v3i t0 = {p0[0],p0[1],p0[2]};
    v3i t1 = {p1[0],p1[1],p1[2]};
    v3i t2 = {p2[0],p2[1],p2[2]}; 
    int z_buff_id;
    int z;
    int x_dir_flag = 0;

    //sort

    if (t0[1] > t1[1]) {
        v3i_swap (t0,t1);
    }
    if (t0[1] > t2[1]) {
        v3i_swap (t0,t2);
    }
    if (t1[1] > t2[1]) {
        v3i_swap (t1,t2);
    }
    int height =  t2[1]-t0[1];
    double seg_height;
    for (int y = t0[1]; y <= t1[1]; y++){
        v3i A,B;
        double alpha;
        double beta;
        alpha = (double) (y-t0[1])/(double)height;
        beta = (double) (y-t0[1])/(double)(t1[1] - t0[1]);
        
        v3i_add(A,t2,t0,-1);//A=t2-t0
        v3i_mul(A,alpha);//A*alpha
        v3i_add(A,A,t0,1);//A+t0;

        v3i_add(B,t1,t0,-1);//B=t1-t0
        v3i_mul(B,beta);//A*beta
        v3i_add(B,B,t0,1);//B+t0;
        
        if (A[0] > B[0]) {
            v3i_swap (A,B);
        }
        for (int xi = A[0]; xi < B[0]; xi++){
            //bilinear
            double ph = A[0] == B[0] ? 1.0 : (double)(xi - A[0])/(double)(B[0]-A[0]);
            v3i P;
            for (int i = 0; i < 3; i++){
                P[i] = A[i] + ((B[i]-A[i]) * ph+0.5);
            }
            int idz = P[0]+P[1]*img->header->width;
            if (z_buff[idz] < P[2]){
                set_pixel(img,P[0],P[1],&color);
                z_buff[idz] = P[2];
            }

        }
    }
    for (int y = t1[1]; y <= t2[1]; y++){
        v3i A,B;
        double alpha;
        double beta;
        alpha = (double) (y-t0[1])/(double)height;
        beta = (double) (y-t1[1])/(double)(t2[1] - t1[1]);
        v3i_add(A,t2,t0,-1);//A=t2-t0
        v3i_mul(A,alpha);//A*alpha
        v3i_add(A,A,t0,+1);//A+t0;

        v3i_add(B,t2,t1,-1);//B=t1-t0
        v3i_mul(B,beta);//A*beta
        v3i_add(B,B,t1,+1);//B+t0;
        if (A[0] > B[0]) {
            v3i_swap (A,B);
        }
        for (int xi = A[0]; xi < B[0]; xi++){
            //bilinear
            double ph = A[0] == B[0] ? 1.0 : (double)(xi - A[0])/(double)(B[0]-A[0]);
            v3i P;
            for (int i = 0; i < 3; i++){
                P[i] = A[i] + ((B[i]-A[i]) * ph+0.5);
            }
            int idz = P[0]+P[1]*img->header->width;
            //z_buff[idz] = P[2];
            if (z_buff[idz] < P[2]){
                set_pixel(img,P[0],P[1],&color);
                z_buff[idz] = P[2];
            }
        }

    }
    return 0;
}


int triangle_face  (tga_image *img, model * mdl, face  * poly, int * z_buff){
    v3i t0, t1, t2;
    v3i screen_coords[3];
    v3d world_coords[3];
    v3d ab;
    v3d bc;
    tga_color color;
    tga_color color2;
    for (int i = 0; i < 3; i++){
        screen_coords[i][0] = (mdl->vertices[poly->vertices[i]][0]+1) * img->header->width/2;
        screen_coords[i][1] = (mdl->vertices[poly->vertices[i]][1]+1) * img->header->height/2;
        screen_coords[i][2] = (mdl->vertices[poly->vertices[i]][2]+1) * Z_DEPTH/2;
        world_coords[i][0] = mdl->vertices[poly->vertices[i]][0]+1;
        world_coords[i][1] = mdl->vertices[poly->vertices[i]][1]+1;
        world_coords[i][2] = mdl->vertices[poly->vertices[i]][2]+1;
    }
    for (int i = 0; i < 3; i++){
       ab[i] = world_coords[2][i] - world_coords[0][i];
       bc[i] = world_coords[1][i] - world_coords[0][i];
    }
    v3d_cross_prod(ab,ab,bc);
    v3d_norm(ab);
    v3d v_light = {-1,0,0};
    double intensity = v3d_dot_prod(v_light,ab);
    if (intensity > 0){
        triangle(img,screen_coords[0],screen_coords[1],screen_coords[2],intensity, z_buff);
        return 1;
    }
    return 0;
}

int get_line_i_z (v3i p1, v3i p2, int x){
    v3i a;
    v3i b;
    if (p1[2] > p2[2]){
        v3i_set(a,p1);
        v3i_set(b,p2);
    } else {
        v3i_set(b,p1);
        v3i_set(a,p2);
    }
    double k = 0;
    int c = b[2];
    if (a[0] - b[0] == 0){
        k = 1;
    } else 
        k = (double)(x - b[0])/(double) (a[0] - b[0]);
    return ((double) (a[2] - b[2]))*k+c;
}

void dump_z_buff (tga_image * img, int * z_buff){
    tga_image tga;
    tga_color color;
    tga = *create_tga(img->header->height,img->header->width);
    for (int i = 0; i < img->header->height; i++)
        for (int j = 0; j < img->header->width; j++){
            set_color (&color,RAW, z_buff[i+j*img->header->width],0,0,0);
            if (z_buff[i+j*img->header->width] == INT_MIN)
                set_color (&color,RAW, 255,0,255,0);
            set_pixel(&tga, i, j, &color);
        }
    write_tga("z_dump.tga", &tga);
}
