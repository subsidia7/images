#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library

typedef struct ImagePGM
{
	char name[50];
	char format[3];
	int width;
	int height;
	int max_depth;
	int** data;
} ImagePGM;

int correct(char* format)
{
	if (format[0] == 'P' && format[1] == '2' && format[2] == '\0')
		return 1;
	return 0;
}

/*
	0 - error
*/
ImagePGM* create_image()
{
	ImagePGM* img = (ImagePGM*)malloc(sizeof(ImagePGM));
	img->data = NULL;
	img->width = 0;
	img->height = 0;
	img->format[0] = '\0';
	img->name[0] = '\0';
	return img;
}

int allocmem(ImagePGM* img)
{
	if (!img)
		return 0;

	img->data = (int**)malloc(img->height * sizeof(int*));
	for (int i = 0; i < img->height; ++i)
	{
		img->data[i] = (int*)malloc(img->width * sizeof(int));
	}
	return 1;
}

int clear(ImagePGM** img)
{
	if (!img)
		return 0;
	if ((*img)->data == NULL)
		return 1;
	for (int i = 0; i < (*img)->height; ++i)
	{
		free((*img)->data[i]);
	}
	return 1;
}

int deleteimg(ImagePGM** img)
{
	if (!img)
		return 0;
	clear(img);
	free(*img);
	return 1;
}

/*
	1 - success
	0 - error
*/
int load_image(const char* filename, ImagePGM* img)
{
	if (!img)
		return 0;
	FILE* file = fopen(filename, "rb");
	if (!file)
		return 0;

	strcpy(img->name, filename);

	fscanf(file, "%s", img->format);
	if (!correct(img->format))
		return 0;

	fscanf(file, "%d", &img->width);
	fscanf(file, "%d", &img->height);
	fscanf(file, "%d", &img->max_depth);

	allocmem(img);

	for (int i = 0; i < img->height; ++i)
	{
		for (int j = 0; j < img->width; ++j)
		{
			fscanf(file, "%d", &img->data[i][j]);
		}
	}

	fclose(file);
	return 1;
}

int printdata(const ImagePGM* img)
{
	if (!img)
		return 0;
	for (int i = 0; i < img->height; ++i)
	{
		for (int j = 0; j < img->width; ++j)
		{
			printf("%d ", img->data[i][j]);
		}
		printf("\n");
	}
	return 1;
}

int rotate90(ImagePGM** img)
{
	if (!img)
		return 0;

	ImagePGM* rotated = create_image();
	strcpy(rotated->name, (*img)->name);
	strcpy(rotated->format, (*img)->format);
	rotated->width = (*img)->height;
	rotated->height = (*img)->width;
	rotated->max_depth = (*img)->max_depth;
	allocmem(rotated);

	int last_row = (*img)->height - 1;
	for (int i = 0; i < rotated->height; ++i)
	{
		for (int j = 0; j < rotated->width; ++j)
		{
			rotated->data[i][j] = (*img)->data[last_row - j][i];
		}
	}
	deleteimg(img);
	*img = rotated;
	return 1;
}

int write_to_file(ImagePGM* img, const char* filename)
{
	FILE* file;
	if (filename == 0)
	{
		char name[] = "rotated_";
		strcat(name, img->name);
		file = fopen(name, "wb");
	}
	else file = fopen(filename, "wb");
	fprintf(file, "%s\n", img->format);
	fprintf(file, "%d %d\n", img->width, img->height);
	fprintf(file, "%d\n", img->max_depth);
	for (int i = 0; i < img->height; ++i)
	{
		for (int j = 0; j < img->width; ++j)
		{
			fprintf(file, "%d ", img->data[i][j]);
		}
		fprintf(file, "\n");
	}
	fclose(file);
	return 1;
}

int main()
{
	const char* filename = "spectr.pgm";
	ImagePGM* img = create_image();
	load_image(filename, img);
	//printdata(img);
	rotate90(&img);
	//printdata(img);
	write_to_file(img, 0);
	return 0;
}