#include <stdio.h>
#include <stdlib.h>

#include "bmp16.h"
#include "constants.h"
#include "file.h"

bmp16* bmp16_create_new(int id, int width, int height)
{
	bmp16* bmp = malloc(sizeof(bmp16));

	bmp->header.id = id;
	bmp->header.size = 14;
	bmp->header.non1 = 0;
	bmp->header.non2 = 0;
	bmp->header.pixel_array_offset = 54;
	
	bmp->dib_size = 40;
	bmp->dib.width = width;
	bmp->dib.height = height;
	bmp->dib.col_planes = 1;
	bmp->dib.bits = 24;
	bmp->dib.bi_rgb = 0;
	bmp->dib.raw_size = width * height * BYTES_PER_PIXEL_24;
	bmp->dib.dpi_horizontal = (int)(72 * 39.3701);
	bmp->dib.dpi_vertical = (int)(72 * 39.3701);
	bmp->dib.col_palette = 0;
	bmp->dib.col_important = 0;

	bmp->pixel_count = width * height;
	bmp->pad = 0;
	int row_bytes = width * BYTES_PER_PIXEL_24;
	if ((row_bytes) % 4 != 0)
		bmp->pad = 4 - (row_bytes % 4);

	bmp->pixels = malloc(sizeof(bmp16_pixel) * bmp->pixel_count);
	
	return bmp;
}

bmp16* bmp16_single_read(const char* file)
{
	bmp16* bmp = malloc(sizeof(bmp16));
	FILE* fd = fopen(file, "rb+");
	
	long fsize = file_size(fd);
	unsigned char* data = malloc(sizeof(unsigned char) * fsize);
	int fres = fread(data, sizeof(unsigned char), fsize, fd);
	fclose(fd);
	
	if (fres != fsize)
		return 0;

	bmp->header.id = file_read_short(&data);
	bmp->header.size = file_read_int(&data);
	bmp->header.non1 = file_read_short(&data);
	bmp->header.non2 = file_read_short(&data);
	bmp->header.pixel_array_offset = file_read_int(&data);
	
	bmp->dib_size = file_read_int(&data);
	bmp->dib.width = file_read_int(&data);
	bmp->dib.height = file_read_int(&data);
	bmp->dib.col_planes = file_read_short(&data);
	bmp->dib.bits = file_read_short(&data);
	bmp->dib.bi_rgb = file_read_int(&data);
	bmp->dib.raw_size = file_read_int(&data);
	bmp->dib.dpi_horizontal = file_read_int(&data);
	bmp->dib.dpi_vertical = file_read_int(&data);
	bmp->dib.col_palette = file_read_int(&data);
	bmp->dib.col_important = file_read_int(&data);

	int i, j, ind = 0;
	int width = bmp->dib.width;
	int height = bmp->dib.height;
	bmp->pixel_count = width * height;
	bmp->pad = 0;
	int row_bytes = width * BYTES_PER_PIXEL_24;
	if ((row_bytes) % 4 != 0)
		bmp->pad = 4 - (row_bytes % 4);
	printf("1.Debug\n");
	printf("PAD: %i\n", bmp->pad);
	bmp->pixels = malloc(sizeof(bmp16_pixel) * bmp->pixel_count);

	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			bmp->pixels[ind++] = bmp16_get_pixel(&data);
		}
		file_skip_pad(&data, bmp->pad);
	}

	return bmp;
}

bmp16_pixel bmp16_get_pixel(unsigned char** data)
{
	bmp16_pixel pixel;
	char components[3];
	components[0] = *((*data)++);
	components[1] = *((*data)++);
	components[2] = *((*data)++);
	int col = *((int*)components);

	pixel.b = (col & 0x00FF0000) >> 16;
	pixel.g = (col & 0x0000FF00) >> 8;
	pixel.r = (col & 0x000000FF);
	
	return pixel;
}

void bmp16_save(bmp16* bmp)
{
	FILE* fd = fopen("output/result.bmp", "wb+");
	int file_size = sizeof(char) * 54 + sizeof(char)*((bmp->dib.width * bmp->dib.height * 3) + bmp->dib.width * bmp->pad);
	unsigned char* file = malloc(file_size);
	unsigned char* file_cpy = file;
	unsigned char pad[2] = {0,0};
	int debug = 0;
	bmp16_pixel* pix_cpy = bmp->pixels;
	
	file_write_data(&file_cpy, (unsigned char*)&bmp->header.id, sizeof(short));
	file_write_data(&file_cpy, (unsigned char*)&bmp->header.size, sizeof(int));
	file_write_data(&file_cpy, (unsigned char*)&bmp->header.non1, sizeof(short));
	file_write_data(&file_cpy, (unsigned char*)&bmp->header.non2, sizeof(short));
	file_write_data(&file_cpy, (unsigned char*)&bmp->header.pixel_array_offset, sizeof(int));
	file_write_data(&file_cpy, (unsigned char*)&bmp->dib_size, sizeof(int));
	file_write_data(&file_cpy, (unsigned char*)&bmp->dib, sizeof(bmp16_dib));
	
	int width = bmp->dib.width;
	int height = bmp->dib.height;
	int i, j;
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			bmp16_pixel pixel = *pix_cpy++;
			int i_pix = 0;

			i_pix |= pixel.r << 16;
			i_pix |= pixel.g << 8;
			i_pix |= pixel.b;

			file_write_data(&file_cpy, (unsigned char*)&i_pix, 3);
		}

		if (bmp->pad > 0)
			file_write_data(&file_cpy, pad, sizeof(unsigned char) * bmp->pad);
	}
	
	fwrite(file, sizeof(unsigned char), file_size, fd);
	perror("Err");
	fclose(fd);
}

bmp16_pixel bmp16_get_pixel_at(bmp16* bmp, int i, int j)
{
	int index = (i * bmp->dib.width) + j;
	
	return bmp->pixels[index];
}

void bmp16_set_pixel_at(bmp16* bmp, bmp16_pixel pixel, int i, int j)
{
	int index = (i * bmp->dib.width) + j;
	bmp->pixels[index] = pixel;
}