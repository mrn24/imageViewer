//Image Viewer

#include "imageViewer.h"
#include "linemath.h"

Image *image;

Vertex vertices[] = {
	{ { -1, 1 },{ 0, 0 } },
	{ { 1, 1 },{ 1, 0 } },
	{ { -1, -1 },{ 0, 1 } },
	{ { 1, 1 },{ 1, 0 } },
	{ { 1, -1 },{ 1, 1 } },
	{ { -1, -1 },{ 0, 1 } }
};

const GLubyte Indices[] = {
	0, 1, 2,
	2, 3, 0
};

char* vertex_shader_src =
"uniform mat4 MVP;\n"
"attribute vec2 TexCoordIn;\n"
"attribute vec2 vPos;\n"
"varying vec2 TexCoordOut;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
"    TexCoordOut = TexCoordIn;\n"
"}\n";


char* fragment_shader_src =
"varying vec2 TexCoordOut;\n"
"uniform sampler2D Texture;\n"
"void main()\n"
"{\n"
"    gl_FragColor = texture2D(Texture, TexCoordOut);\n"
"}\n";



//Read P3 and P6
int read_ppm(char *input_file) {
	FILE *fp;
	int n, c, i, j, k;

	fp = fopen(input_file, "r");
	if (fp == 0) {
		fp = fopen(input_file, "rb");
	}

	if (fp == 0) {
		fclose(fp);
		fprintf(stderr, "Error: Unable to open file\n");
		exit(1);
	}

	c = fgetc(fp);
	if (c != P) {
		fprintf(stderr, "Error: Filetype incompatible\n");
		exit(1);
	}


	image = (Image*)malloc(sizeof(Image));
	if (image == 0) {
		fprintf(stderr, "Error: Memory allocation unsuccessful\n");
		exit(1);
	}

	c = fgetc(fp);

	image->format = c;

	if (fscanf(fp, "%d %d", &image->width, &image->height) != 2) {
		fprintf(stderr, "Error: Width and Height missing\n");
		exit(1);
	}

	fscanf(fp, "%d", &image->range);

	if (image->range != MAX_COLORS) {
		fprintf(stderr, "Error: Expected range of %d not found\n", MAX_COLORS);
		exit(1);
	}

	image->buffer = (RGBpixel*)malloc((MAX_COLORS + 1)*image->width * image->height);

	if (image->buffer == 0) {
		fprintf(stderr, "Error: Memory allocation unsuccessful\n");
		exit(1);
	}

	fgetc(fp);
	if (image->format == '3') {
		i = 0;

		for (i = 0; i < image->height * image->width; i++) {
			fscanf(fp, "%d", &n);
			image->buffer[i].r = n;
			fscanf(fp, "%d", &n);
			image->buffer[i].g = n;
			fscanf(fp, "%d", &n);
			image->buffer[i].b = n;
		}
	}
	if (image->format == '6') {
		fread(image->buffer, (sizeof(&image->buffer)), image->height * image->width, fp);
	}
	fclose(fp);
	return 0;
}


int main(int argc, char *argv[]) {





	return 0;
}
