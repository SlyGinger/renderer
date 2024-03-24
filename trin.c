#include "trin.h"

int triangle (tga_image * img, v3i p0, v3i p1, v3i p2, double intensity, int * z_buff){
    printf ("strating tring\n");
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
        // swap_int(&t0[1],&t1[1]);
        // swap_int(&t0[0],&t1[0]);
        v3i_swap (t0,t1);
    }
    if (t0[1] > t2[1]) {
        // swap_int(&t0[1],&t2[1]);
        // swap_int(&t0[0],&t2[0]);
        v3i_swap (t0,t2);
    }
    if (t1[1] > t2[1]) {
        // swap_int(&t1[1],&t2[1]);
        // swap_int(&t1[0],&t2[0]);
        v3i_swap (t1,t2);
    }
    int height =  t2[1]-t0[1];
    double seg_height;
    for (int y = t0[1]; y <= t1[1]; y++){
        v3i A,B;
        double alpha;
        double beta;
        alpha = (double) (y-t0[1])/(double)height;
        //printf ("A=%lf\n",alpha);
        beta = (double) (y-t0[1])/(double)(t1[1] - t0[1]);
        
        v3i_add(A,t2,t0,-1);//A=t2-t0
        v3i_mul(A,alpha);//A*alpha
        v3i_add(A,A,t0,1);//A+t0;

        v3i_add(B,t1,t0,-1);//B=t1-t0
        v3i_mul(B,beta);//A*beta
        v3i_add(B,B,t0,1);//B+t0;
        if (t0[1] == t1[1]){
            printf ("DATA\n");
            print_v3i(t0);
            print_v3i(t1);
            print_v3i(t2);
            printf ("alpha = %lf beta = %lf\n",alpha,beta); 
            print_v3i(A);
            print_v3i(B);
        }
        
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
            // v3i_add(P,A,B,-1);//P = A-B
            // v3i_mul(P,ph);//P*ph
            // v3i_add(P,P,A,1);//P+A;

            int idz = P[0]+P[1]*img->header->width;
            //z_buff[idz] = P[2];
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
        if (t2[1] == t1[1]){
            printf ("DATA2\n");
            print_v3i(t0);
            print_v3i(t1);
            print_v3i(t2);
            printf ("alpha = %lf beta = %lf\n",alpha,beta); 
            print_v3i(A);
            print_v3i(B);
        }
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
            // v3i_add(P,A,B,-1);//P = A-B
            // v3i_mul(P,ph);//P*ph
            // v3i_add(P,P,A,1);//P+A;


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
    
    printf ("_________________\n");
    for (int i = 0; i < 3; i++){

        screen_coords[i][0] = (mdl->vertices[poly->vertices[i]][0]+1) * img->header->width/2;
        screen_coords[i][1] = (mdl->vertices[poly->vertices[i]][1]+1) * img->header->height/2;
        screen_coords[i][2] = (mdl->vertices[poly->vertices[i]][2]+1) * Z_DEPTH/2;
        world_coords[i][0] = mdl->vertices[poly->vertices[i]][0]+1;
        world_coords[i][1] = mdl->vertices[poly->vertices[i]][1]+1;
        world_coords[i][2] = mdl->vertices[poly->vertices[i]][2]+1;
        
        printf ("wc: ");
        print_v3d(world_coords[i]);
        printf ("sc: ");
        print_v3i(screen_coords[i]);
        
        
    }
   // printf ("_________________\n");
    
    for (int i = 0; i < 3; i++){
        printf ("ab expected: %lf=%lf-%lf ",world_coords[2][i] - world_coords[0][i],world_coords[2][i], world_coords[0][i]);
       // ab[i] = world_coords[2][i] - world_coords[0][i];
       // bc[i] = world_coords[1][i] - world_coords[0][i];
       ab[i] = world_coords[2][i] - world_coords[0][i];
       bc[i] = world_coords[1][i] - world_coords[0][i];
    }
   printf ("\nab: ");
   print_v3d(ab);
   printf ("bc: ");
   print_v3d(bc);
    v3d_cross_prod(ab,ab,bc);
    v3d_norm(ab);
    
    v3d v_light = {-1,0,0};
    double intensity = v3d_dot_prod(v_light,ab);
    print_v3d(ab);
    printf ("t3 int = %lf\n", intensity);
    if (intensity > 0){
        triangle(img,screen_coords[0],screen_coords[1],screen_coords[2],intensity, z_buff);
    }
    
    return 0;
}


int get_line_i_x(v3i a, v3i b, int y){
    double k = 0;
    v3i t0 = {a[0],a[1],a[2]};
    v3i t1 = {b[0],b[1],b[2]};
    if (t0[0] < t1[0]){
        v3i_swap(t0,t1);
    }
    
    k =  ((double) (t0[1] - t1[1])) / (double) (t0[0] - t1[0]);
    //handled
    if (t0[0] - t1[0] == 0){
        if (t0[1] - t1[1]==0){
            printf ("SAME_POINTS_ERR_X_FN k = %lf\n",k);
        } else {
            printf ("DIFF_POINTSERR_X_FN k = %lf\n",k);
        }
        return -999;
        
    }
    double c = t0[1] - k * t0[0];
    //printf ("!linear function: %d = %lf * x + %lf (%lf %lf)\n",y,k,c,(double) (t0[1] - t1[1]),(double) (t0[0] - t1[0]));
    //printf ("!linear function x%lf\n", (y - c)/k);
    if (t0[0] - t1[0] == 0)
        printf ("ERR_X_FN x = %lf\n",(y - c)/k);
    return (y - c)/k;
}

int get_line_i_y (v3i a, v3i b, int x){
    double k = 0;
    int c = a[1] > b[1] ? b[1] : a[1];
    k =  ((double) (a[1] - b[1])) / (double) (a[0] - b[0]);
    printf ("k=%lf c=%d\n",k,c);
    return x*k+c;
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
    //int c = a[2] > b[2] ? b[2] : a[2];
    int c = b[2];
    
    //k =  ((double) (a[2] - b[2])) / (double) (a[0] - b[0]);
     
    if (a[0] - b[0] == 0){
        k = 1;
        //k=200;
        printf ("possible wrong dx=0, but z = %lf k=%lf c = %d\n",x*k+c, k, c);
        //return 20000;
        
    } else 
        k = (double)(x - b[0])/(double) (a[0] - b[0]);
   // if (a[2] - b[2] == 0){
   //     k=200;
   //     printf ("possible wrong dz=0, but z = %lf k=%lf c = %d\n",x*k+c, k, c);
   //     return 10000;
        
   // }
        //k =  200;
  //  if (a[2] - b[2] == 0)
   //     printf ("possible wrong dz=0, but z = %lf k=%lf c = %d\n",x*k+c, k, c);
    printf ("Z: k=%lf c=%d\n",k,c);
    return ((double) (a[2] - b[2]))*k+c;
}

void print_v2i (v2i a){
    printf ("v2i x:%d y:%d\n", a[0],a[1]);
}

void dump_z_buff (tga_image * img, int * z_buff){
    tga_image tga;
    tga_color color;
    printf ("DUNMPING Z_BUFF\n");
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

int linear_interpolate (v3i a, v3i b, int arg){
    int f0;
    int f1;
    int x0;
    int x1;
    f0 = a[2];

}

int bilinear_interpolation (v3i a, v3i b, int x, int y){
    

}