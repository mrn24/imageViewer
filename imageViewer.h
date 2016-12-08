//Image Viewer Header File

#ifdef _APPLE_
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <GLES2/gl2.h>
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
	float position[3];
	float color[4];
} Vertex;
