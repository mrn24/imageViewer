//Image Viewer Header File


#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>

#define PI 3.14159265
#define MAX_COLORS 255

typedef struct RGBpixel {
	unsigned char r, g, b;
} RGBpixel;

typedef struct Image {
	int width, height, format, range;
	RGBpixel *buffer;
}Image;

typedef struct {
	float Position[2];
	float TexCoord[2];
} Vertex;
