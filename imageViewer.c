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

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

float rotate = 0;
float scale = 1;
float sheer = 0;
float size = 0;
float translateX = 0;
float translateY = 0;


static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
		switch (key) {
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GLFW_TRUE);
			break;
		case GLFW_KEY_Q:
			rotate += PI / 4;
			break;
		case GLFW_KEY_E:
			rotate -= PI / 4;
			break;
		case GLFW_KEY_W:
			translateY += 1;
			break;
		case GLFW_KEY_S:
			translateY -= 1;
			break;
		case GLFW_KEY_A:
			translateX -= 1;
			break;
		case GLFW_KEY_D:
			translateX += 1;
			break;
		case GLFW_KEY_UP:
			scale += 1;
			break;
		case GLFW_KEY_DOWN:
			scale -= 1;
			if (scale < 0) {
				scale = 0;
			}
			break;
		}
		//QE - Rotate
		//WASD - Translation
		//uparrow/downarrow - scale--
}

void glCompileShaderOrDie(GLuint shader) {
	GLint compiled;
	glCompileShader(shader);
	glGetShaderiv(shader,
		GL_COMPILE_STATUS,
		&compiled);
	if (!compiled) {
		GLint infoLen = 0;
		glGetShaderiv(shader,
			GL_INFO_LOG_LENGTH,
			&infoLen);
		char* info = malloc(infoLen + 1);
		GLint done;
		glGetShaderInfoLog(shader, infoLen, &done, info);
		printf("Unable to compile shader: %s\n", info);
		exit(1);
	}
}

/*static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void glCompileShaderOrDie(GLuint shader) {
  GLint compiled;
  glCompileShader(shader);
  glGetShaderiv(shader,
		GL_COMPILE_STATUS,
		&compiled);
  if (!compiled) {
    GLint infoLen = 0;
    glGetShaderiv(shader,
		  GL_INFO_LOG_LENGTH,
		  &infoLen);
    char* info = malloc(infoLen+1);
    GLint done;
    glGetShaderInfoLog(shader, infoLen, &done, info);
    printf("Unable to compile shader: %s\n", info);
    exit(1);
  }
}

// 4 x 4 image..
unsigned char image[] = {
  255, 0, 0, 255,
  255, 0, 0, 255,
  255, 0, 0, 255,
  255, 0, 0, 255,

  0, 255, 0, 255,
  0, 255, 0, 255,
  0, 255, 0, 255,
  0, 255, 0, 255,

  0, 0, 255, 255,
  0, 0, 255, 255,
  0, 0, 255, 255,
  0, 0, 255, 255,

  255, 0, 255, 255,
  255, 0, 255, 255,
  255, 0, 255, 255,
  255, 0, 255, 255
};

int main(void)
{
    GLFWwindow* window;
    GLuint vertex_buffer, vertex_shader, fragment_shader, program;
    GLint mvp_location, vpos_location, vcol_location;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);
    // gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSwapInterval(1);

    // NOTE: OpenGL error checks have been omitted for brevity

    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexes), vertexes, GL_STATIC_DRAW);

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShaderOrDie(vertex_shader);

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShaderOrDie(fragment_shader);

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    // more error checking! glLinkProgramOrDie!

    mvp_location = glGetUniformLocation(program, "MVP");
    assert(mvp_location != -1);

    vpos_location = glGetAttribLocation(program, "vPos");
    assert(vpos_location != -1);

    GLint texcoord_location = glGetAttribLocation(program, "TexCoordIn");
    assert(texcoord_location != -1);

    GLint tex_location = glGetUniformLocation(program, "Texture");
    assert(tex_location != -1);

    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location,
			  2,
			  GL_FLOAT,
			  GL_FALSE,
                          sizeof(Vertex),
			  (void*) 0);

    glEnableVertexAttribArray(texcoord_location);
    glVertexAttribPointer(texcoord_location,
			  2,
			  GL_FLOAT,
			  GL_FALSE,
                          sizeof(Vertex),
			  (void*) (sizeof(float) * 2));

    int image_width = 4;
    int image_height = 4;

    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA,
		 GL_UNSIGNED_BYTE, image);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texID);
    glUniform1i(tex_location, 0);

    while (!glfwWindowShouldClose(window))
    {
        float ratio;
        int width, height;
        mat4x4 m, p, mvp;

        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        mat4x4_identity(m);
        mat4x4_rotate_Z(m, m, (float) glfwGetTime());
        mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        mat4x4_mul(mvp, p, m);

        glUseProgram(program);
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

//! [code]*/


int main(int argc, char *argv[]) {

	if (argc != 2) {
		fprintf(stderr, "ERROR: Incorrect amount of input arguments recieved\n");
		exit(1);
	}

	char *picture = argv[1];
	read_ppm(picture);

	size = (float)image->width / (float)image->height;
	vertices[0].Position[0] *= size;
	vertices[1].Position[0] *= size;
	vertices[2].Position[0] *= size;
	vertices[3].Position[0] *= size;
	vertices[4].Position[0] *= size;
	vertices[5].Position[0] *= size;

	GLFWwindow* window;
	GLuint vertex_buffer, vertex_shader, fragment_shader, program;
	GLint mvp_location, vpos_location, vcol_location;

	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
		exit(EXIT_FAILURE);

	window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(window, key_callback);

	glfwMakeContextCurrent(window);
	// gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
	glfwSwapInterval(1);

	// NOTE: OpenGL error checks have been omitted for brevity

	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
	glCompileShaderOrDie(vertex_shader);

	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
	glCompileShaderOrDie(fragment_shader);

	program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);
	// more error checking! glLinkProgramOrDie!

	mvp_location = glGetUniformLocation(program, "MVP");
	assert(mvp_location != -1);

	vpos_location = glGetAttribLocation(program, "vPos");
	assert(vpos_location != -1);

	GLint texcoord_location = glGetAttribLocation(program, "TexCoordIn");
	assert(texcoord_location != -1);

	GLint tex_location = glGetUniformLocation(program, "Texture");
	assert(tex_location != -1);

	glEnableVertexAttribArray(vpos_location);
	glVertexAttribPointer(vpos_location,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)0);

	glEnableVertexAttribArray(texcoord_location);
	glVertexAttribPointer(texcoord_location,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(sizeof(float) * 2));


	GLuint texID;
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, image->buffer);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texID);
	glUniform1i(tex_location, 0);

	while (!glfwWindowShouldClose(window))
	{
		float ratio;
		int width, height;
		mat4x4 m, p, mvp;

		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;

		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);

		mat4x4_identity(m);
		mat4x4_rotate_Z(m, m, (float)glfwGetTime());
		mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
		mat4x4_mul(mvp, p, m);

		glUseProgram(program);
		glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)mvp);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}
