#include "tga.h"

tga_image* read_tga (const char * filename){
    FILE *f;
    f = fopen(filename, "rb");
    tga_hdr *header;
    tga_image *img;
    header = (tga_hdr *) malloc (sizeof(tga_hdr));
    img = (tga_image *) malloc (sizeof(tga_image));
    
    fread((char *) &header->id_length  ,sizeof(char),1,f);
    fread((char *) &header->colormap_type  ,sizeof(char),1,f);
    fread((char *) &header->image_type  ,sizeof(char),1,f);
    fread((char *) &header->colormap_first_entry_index  ,sizeof(short),1,f);
    fread((char *) &header->colormap_length  ,sizeof(short),1,f);
    fread((char *) &header->colormap_map_entry_size  ,sizeof(char),1,f);
    fread((char *) &header->x_origin  ,sizeof(short),1,f);
    fread((char *) &header->y_origin  ,sizeof(short),1,f);
    fread((char *) &header->width  ,sizeof(short),1,f);
    fread((char *) &header->height  ,sizeof(short),1,f);
    fread((char *) &header->pixel_depth  ,sizeof(char),1,f);
    fread((char *) &header->image_descriptor  ,sizeof(char),1,f);
    print_metadata(header);
    img->ppd = header->pixel_depth>>3;
    img->data = (uint8_t *) malloc (header->width * header->height * img->ppd);
    fread((uint8_t *) img->data  ,sizeof(uint8_t) * 3,header->width * header->height * img->ppd,f);
    img->header = header;
    //printf ("n=%d p=%d div=%lu s=%lu", img->ppd, header->pixel_depth, header->pixel_depth/sizeof(uint8_t), sizeof(uint8_t));
    //printf ("read: %lu\n", fread(  (uint8_t *)img->data, sizeof(uint8_t), header->width * header->height * img->ppd, f ));
    //printf ("pixels: %d bytes: %d \n", header->width * header->height, header->width * header->height * header->pixel_depth /8 );
    //printf ("header struct: %ld \n", sizeof (tga_hdr));
    fclose(f);
    return img;
}

void set_tga_header (tga_hdr * header, const char * str){
    
}
void print_metadata (tga_hdr * hdr){
    printf ("metadata:\n________________\n");
    printf ("char id_length: %d\n", hdr->id_length);
	printf ("char colormap_type %d\n", hdr->colormap_type);
	printf ("char image_type %d\n", hdr->image_type);
	printf ("short colormap_first_entry_index %d\n", hdr->colormap_first_entry_index);
	printf ("short colormap_length %d\n", hdr->colormap_type);
	printf ("char colormap_map_entry_size %d\n", hdr->colormap_map_entry_size);
	printf ("short x_origin %d\n", hdr->x_origin);
	printf ("short y_origin %d\n", hdr->y_origin);
	printf ("short width %d\n", hdr->width);
	printf ("short height %d\n", hdr->height);
	printf ("char  pixel_depth %d\n", hdr->pixel_depth);
	printf ("char  image_descriptor %d\n", hdr->image_descriptor);
}

void write_tga (const char * filename, tga_image * img ){
    FILE *f;
    f = fopen(filename, "wb");
    fwrite(&img->header->id_length, sizeof(char),1 , f);
    fwrite(&img->header->colormap_type  ,sizeof(char),1,f);
    fwrite(&img->header->image_type  ,sizeof(char),1,f);
    fwrite(&img->header->colormap_first_entry_index  ,sizeof(short),1,f);
    fwrite(&img->header->colormap_length  ,sizeof(short),1,f);
    fwrite(&img->header->colormap_map_entry_size  ,sizeof(char),1,f);
    fwrite(&img->header->x_origin  ,sizeof(short),1,f);
    fwrite(&img->header->y_origin  ,sizeof(short),1,f);
    fwrite(&img->header->width  ,sizeof(short),1,f);
    fwrite(&img->header->height  ,sizeof(short),1,f);
    fwrite(&img->header->pixel_depth  ,sizeof(char),1,f);
    fwrite(&img->header->image_descriptor  ,sizeof(char),1,f);
    fwrite(img->data  ,sizeof(uint8_t), img->header->width * img->header->height * img->ppd,f);
    fclose(f);
}

void cross (tga_image * img){
    int i = 0;
    int j = 0;
    tga_color a;
    set_color(&a,RAW,255,0,255,0);
    for (i = 0; i < img->header->height; i ++)
        for (j = 0; j < img->header->width; j ++){
                if (i == j)  set_pixel(img,i,j,&a);
        }
}

int line (tga_image *img, int x, int y, int xn, int yn){
    tga_color color;
    int steep = 0;
    if (abs(x - xn) < abs(y - yn)){
        steep = 1;
        swap_int (&x,&y);
        swap_int (&xn,&yn);
    }
    if (x > xn){
        swap_int (&x,&xn);
        swap_int (&y,&yn);
    }
    //printf ("x=%d y=%d xn=%d yn=%d\n", x, y, xn,yn);
    set_color (&color,RAW, 255,0,255,0);
    int dx = (xn - x);
    int dy = (yn - y);
    int add_y =  2 * abs(dy);
    int e_dy = 0;
    int current_y = y;
    int current_x = x;
    for ( current_x = x; current_x < xn; current_x ++){
        if (steep){
            set_pixel(img, current_y, current_x, &color);
        } else {
            set_pixel(img, current_x, current_y, &color);
        }
        e_dy += add_y;
        if (e_dy > dx){
            current_y += (yn > y ? 1 : -1);
            e_dy -= dx * 2;
        }
    }
    return 1;
}

int set_color (tga_color *color, uint8_t t, uint8_t r, uint8_t g, uint8_t b, uint8_t a){
    switch (t)
    {
    case GREY:
        color->type = t;
        color->rav_grey = r;
        break;
    case RAW:
        color->type = t;
        color->rav_rgb[0] = r;
        color->rav_rgb[1] = g;
        color->rav_rgb[2] = b;
        break;
    case RAW_A:
        color->type = t;
        color->rav_rgba[0] = r;
        color->rav_rgba[1] = g;
        color->rav_rgba[2] = b;
        color->rav_rgba[3] = a;
        break;
    case STRUCT:
        color->type = t;
        color->rgba.r = r;
        color->rgba.g = g;
        color->rgba.b = b;
        color->rgba.a = a;
        break;
    
    default:
        printf ("wrong colortype\n");
        return 1;
        break;
    return 1;
    }
    return 0;
}
int set_pixel (tga_image * img, int x, int y, tga_color * pixel){
    //check ppd & pixel type
    // if (x > img->header->width){
    //     printf ("!!!WORNG_W trying to set x=%d w=%d\n", x, img->header->width);
    //     return 0;
    // }
    // if (x > img->header->height){
    //     printf ("!!!WORNGh\n");
    //     return 0;
    // }
    //printf ("setpixel: %d %d\n" ,x,y);
    switch (pixel->type)
    {
    case GREY:
        if (x <= img->header->width  && y <= img->header->height)
            memcpy(img->data+((x+y*img->header->width)*img->ppd),&pixel->rav_grey, img->ppd);
        break;
    case RAW_A:
        if (x <= img->header->width  && y <= img->header->height)
            memcpy(img->data+((x+y*img->header->width)*img->ppd),&pixel->rav_rgba, img->ppd);
        break;
    case RAW:
        if (x <= img->header->width  && y <= img->header->height)
            memcpy(img->data+((x+y*img->header->width)*img->ppd),&pixel->rav_rgb, img->ppd);
        break;
    case STRUCT:
        if (x <= img->header->width  && y <= img->header->height)
            memcpy(img->data+((x+y*img->header->width)*img->ppd),&pixel->rgba, img->ppd);
        break;
    
    default:
        printf ("wrong colortype!\n");
        return 1;
        break;
    }
    return 0;
}

void swap_int (int *a, int *b){
    int t = *a;
    *a = *b;
    *b = t;
}

tga_image *create_tga (int width, int height){
    tga_hdr *header;
    tga_image *img;
    header = (tga_hdr *) malloc (sizeof(tga_hdr));
    img = (tga_image *) malloc (sizeof(tga_image));
    header->height = height;
    header->width = width;
    header->image_type = 2;
    header->pixel_depth = 24;
    img->ppd = header->pixel_depth>>3;
    img->data = (uint8_t *) malloc (header->width * header->height * img->ppd);
    memset(img->data,255,header->width * header->height * img->ppd);
    img->header = header;
    return img;
}
