#ifndef BMP16_H
#define BMP16_H

typedef struct header
{
	short id;
	int size;
	short non1;
	short non2;
	int pixel_array_offset;
}bmp_header;


typedef struct pixel
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
}bmp16_pixel;

typedef struct dib
{
	int width;
	int height;
	short col_planes;
	short bits;
	int bi_rgb;
	int raw_size;
	int dpi_horizontal;
	int dpi_vertical;
	int col_palette;
	int col_important;
}bmp16_dib;

/*
	The pixel array should be a seperate structure
	so the get_pixel and set_pixel work independently
	from the bmp structure.
*/
typedef struct bmp
{
	bmp_header header;
	int dib_size;
	bmp16_dib dib;
	bmp16_pixel* pixels;
	int pixel_count;
	int pad;
} bmp16;

/**
 * Creates a new bmp16 structure with predefined header and dib sections.
 * */
bmp16* bmp16_create_new(int id, int width, int height);
/**
 * 
 * */
bmp16* bmp16_single_read(const char* file);
bmp16_pixel bmp16_get_pixel(unsigned char** data);
void bmp16_save(bmp16* bmp);
bmp16_pixel bmp16_get_pixel_at(bmp16* bmp, int i, int j);
void bmp16_set_pixel_at(bmp16* bmp, bmp16_pixel pixel, int i, int j);

#endif