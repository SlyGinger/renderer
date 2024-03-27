#include "trin.h"

int triangle (tga_image * img, v3i t0, v3i t1, v3i t2, v3i uv0, v3i uv1, v3i uv2, model * mdl, double intensity, int * z_buff){
    //printf ("strating tring\n");
    tga_color color;
    tga_color pixel_color;
    tga_color color2;
    tga_color color3;
    set_color (&color,RAW, 255*intensity,255*intensity,255*intensity,0);
    set_color (&color2,RAW, 255,0,255,0);
    set_color (&color3,RAW, 0,255,255,0);
    v3i p0;
    v3i p1;
    v3i p2;
    v3i tx0;
    v3i tx1;
    v3i tx2;
    for (int i = 0; i < 3; i++){
        p0[i] = t0[i];
        p1[i] = t1[i];
        p2[i] = t2[i];
        tx0[i] = uv0[i];
        tx1[i] = uv1[i];
        tx2[i] = uv2[i];
    }
    tx2[2] = tx1[2] = tx0[2] = 0;
    int z_buff_id;
    int z;
    int x_dir_flag = 0;

    //sort



    if (p0[1] > p1[1]) {
        v3i_swap (p0,p1);
        v3i_swap (tx0,tx1);
    }
    if (p0[1] > p2[1]) {
        v3i_swap (p0,p2);
        v3i_swap (tx0,tx2);
    }
    if (p1[1] > p2[1]) {
        v3i_swap (p1,p2);
        v3i_swap (tx1,tx2);
    }
    int height =  p2[1]-p0[1];
    double seg_height;
    //printf (")start uv test: U0:%d V0:%d\n",uv0[0],uv0[1]);
    for (int y = p0[1]; y <= p1[1]; y++){
        v3i A,B,txA,txB;
        double alpha;
        double beta;

        alpha = (double) (y-p0[1])/(double)height;
        beta = (double) (y-p0[1])/(double)(p1[1] - p0[1]);
        
        v3i_add(A,p2,p0,-1);//A=p2-p0
        v3i_mul(A,alpha);//A*alpha
        v3i_add(A,A,p0,1);//A+p0;

        v3i_add(B,p1,p0,-1);//B=p1-p0
        v3i_mul(B,beta);//A*beta
        v3i_add(B,B,p0,1);//B+p0;

        v3i_add(txA,tx2,tx0,-1);//txA=tx2-tx0
        v3i_mul(txA,alpha);//txA*alpha
        v3i_add(txA,txA,tx0,1);//A+tx0;

        v3i_add(txB,tx1,tx0,-1);//txB=tx1-tx0
        v3i_mul(txB,beta);//A*beta
        v3i_add(txB,txB,tx0,1);//txB+tx0;
        
        if (A[0] > B[0]) {
            v3i_swap (A,B);
            v3i_swap (txA,txB);
        }
            //printf ("start uv test: u0:%d v0:%d\n",txA[0],txA[1]);
        for (int xi = A[0]; xi < B[0]; xi++){
            //bilinear
            double ph = A[0] == B[0] ? 1.0 : (double)(xi - A[0])/(double)(B[0]-A[0]);
            v3i P;
            v3i txP;
            for (int i = 0; i < 3; i++){
                P[i] = A[i] + ((B[i]-A[i]) * ph+0.5);
                txP[i] = txA[i] + ((txB[i]-txA[i]) * ph+0.5); 
            }
            int idz = P[0]+P[1]*img->header->width;
            int pixel = (txP[0] + P[1] * mdl->diffuse_width) * 3 ;
            if (z_buff[idz] < P[2]){
                printf ("pixel height:%d width: %d\n",txP[0],txP[1]);
                set_color (&color,RAW, mdl->diffuse_texture[pixel+2] *intensity,mdl->diffuse_texture[pixel+1]*intensity,
                    mdl->diffuse_texture[pixel]*intensity,0);
                set_pixel(img,P[0],P[1],&color);
                z_buff[idz] = P[2];
            }

        }
    }
    for (int y = p1[1]; y <= p2[1]; y++){
        v3i A,B;
        double alpha;
        double beta;
        alpha = (double) (y-p0[1])/(double)height;
        beta = (double) (y-p1[1])/(double)(p2[1] - p1[1]);
        v3i_add(A,p2,p0,-1);//A=p2-p0
        v3i_mul(A,alpha);//A*alpha
        v3i_add(A,A,p0,+1);//A+p0;

        v3i_add(B,p2,p1,-1);//B=p1-p0
        v3i_mul(B,beta);//A*beta
        v3i_add(B,B,p1,+1);//B+p0;
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
                set_pixel(img,P[0],P[1],&color2);
                z_buff[idz] = P[2];
            }
        }

    }
    return 0;
}


int triangle_face  (tga_image *img, model * mdl, face  * poly, int * z_buff){
    v3i screen_coords[3];
    v3d world_coords[3];
    v3i texture_coords[3];

    v3d ab;
    v3d bc;
    v3d t_ab;
    v3d t_bc;
    tga_color color;
    tga_color color2;
    for (int i = 0; i < 3; i++){
        screen_coords[i][0] = (mdl->vertices[poly->vertices[i]][0]+1) * img->header->width/2;
        screen_coords[i][1] = (mdl->vertices[poly->vertices[i]][1]+1) * img->header->height/2;
        screen_coords[i][2] = (mdl->vertices[poly->vertices[i]][2]+1) * Z_DEPTH/2;
        world_coords[i][0] = mdl->vertices[poly->vertices[i]][0]+1;
        world_coords[i][1] = mdl->vertices[poly->vertices[i]][1]+1;
        world_coords[i][2] = mdl->vertices[poly->vertices[i]][2]+1;
        texture_coords[i][0] = (mdl->texture_vertices[poly->texture_vertices[i]][0]+1) * mdl->diffuse_width/2;
        texture_coords[i][1] = (mdl->texture_vertices[poly->texture_vertices[i]][1]+1) * mdl->diffuse_height/2;
        texture_coords[i][2] = mdl->texture_vertices[poly->texture_vertices[i]][2]+1;
        //printf ("scaled: u0:%d v0:%d\n",texture_coords[i][0],texture_coords[i][1]);
    }
    for (int i = 0; i < 3; i++){
       ab[i] = world_coords[2][i] - world_coords[0][i];
       bc[i] = world_coords[1][i] - world_coords[0][i];
       //t_ab[i] = texture_coords[2][i] - texture_coords[0][i];
       //t_bc[i] = texture_coords[1][i] - texture_coords[0][i];
    }
    v3d_cross_prod(ab,ab,bc);
    v3d_norm(ab);
    //v3d_cross_prod(t_ab,t_ab,t_bc);
    //v3d_norm(t_ab);
    v3d v_light = {-1,0,0};
    double intensity = v3d_dot_prod(v_light,ab);
    if (intensity > 0){
        triangle(img,screen_coords[0],screen_coords[1],screen_coords[2],texture_coords[0], texture_coords[1],
            texture_coords[2], mdl, intensity,z_buff);
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
