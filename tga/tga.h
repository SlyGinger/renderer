#ifndef __TGA_H__
#define __TGA_H__

#include "../common.h"
#include "../geometry/geometry.h"

#pragma once

#define RAW_A 0
#define RAW 1
#define GREY 2
#define STRUCT 3
#define WRONG_TYPE 4




typedef struct TGA_Header {
	char id_length; //This field identifies the number of bytes contained in Field 6, the Image ID Field
	char colormap_type; //This field indicates the type of color map (if any) included with the image. 0 - no color map. 1 - + colormap
	char image_type; // 0 - 128 reserved. TGA File can be used to store Pseudo-Color, True-Color and Direct-Color. RLE - compressed.
	short colormap_first_entry_index;
	short colormap_length;
	char colormap_map_entry_size;
	short x_origin;
	short y_origin;
	short width;
	short height;
	char  pixel_depth;
	char  image_descriptor;
} tga_hdr;

typedef struct TGA_Color {
    union 
	{
		struct RGBA {
			uint8_t r,g,b,a;
		} rgba;
		uint8_t rav_rgba[4];
		uint8_t rav_rgb[3];
		uint8_t rav_grey;
	};
	uint8_t type;
	
} tga_color;

typedef struct TGA_Image {
    tga_hdr *header;
    char *image_id;
    char *color_map;
    uint8_t *data;
    char *dev_data;
    //char *extension_data;
	char footer[18];
	int ppd;
	
} tga_image;

int set_color (tga_color* ,uint8_t,uint8_t,uint8_t,uint8_t,uint8_t);
tga_image* read_tga (const char *);
int set_pixel (tga_image *, int, int, tga_color *);
void cross (tga_image *);
void write_tga (const char *, tga_image *);
void print_metadata (tga_hdr *);
void set_tga_header (tga_hdr *,const char *);
void swap_int (int *,int *);
int line (tga_image *, int, int, int, int);
tga_image *create_tga (int, int);
#endif