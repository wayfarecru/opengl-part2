#ifndef __cplusplus
#error This file works only with C++
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <chrono>
#include <iostream>
using namespace std;
using namespace std::chrono;

#pragma warning(disable: 4711 4710 4100 4514 4626 4774 4365 4625 4464 4571 4201 5026 5027 5039)

#define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp> // for glm::to_string()
#include <glm/gtc/type_ptr.hpp> // for glm::value_ptr( )

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glfw3.lib")
#pragma warning(disable: 4711 4710 4100)

#include "./common.c"

const unsigned int WIN_W = 640; // window size in pixels, (Width, Height)
const unsigned int WIN_H = 480;
const unsigned int WIN_X = 100; // window position in pixels, (X, Y)
const unsigned int WIN_Y = 100;

const char* vertFileName = "c52-mvp.vert";
const char* fragFileName = "c52-mvp.frag";

GLuint vert = 0; // vertex shader ID number
GLuint frag = 0; // fragment shader ID number
GLuint prog = 0; // shader program ID number

void initFunc(void) {
	const char* vertSource = loadFile( vertFileName );
	const char* fragSource = loadFile( fragFileName );
	char buf[1024]; // mesg buffer
	GLint status; // for glGetShaderiv()
	// vert: vertex shader
	vert = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert, 1, &vertSource, nullptr);
	glCompileShader(vert); // compile to get .OBJ
	glGetShaderiv(vert, GL_COMPILE_STATUS, &status);
	printf("vert compile status = %s\n", (status == GL_TRUE) ? "true" : "false");
	glGetShaderInfoLog(vert, sizeof(buf), nullptr, buf);
	printf("vert log = [%s]\n", buf);
	// frag: fragment shader
	frag = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag, 1, &fragSource, nullptr);
	glCompileShader(frag); // compile to get .OBJ
	glGetShaderiv(frag, GL_COMPILE_STATUS, &status);
	printf("frag compile status = %s\n", (status == GL_TRUE) ? "true" : "false");
	glGetShaderInfoLog(frag, sizeof(buf), nullptr, buf);
	printf("frag log = [%s]\n", buf);
	// prog: program
	prog = glCreateProgram();
	glAttachShader(prog, vert);
	glAttachShader(prog, frag);
	glLinkProgram(prog); // link to get .EXE
	glGetProgramiv(prog, GL_LINK_STATUS, &status);
	printf("prog link status = %s\n", (status == GL_TRUE) ? "true" : "false");
	glGetProgramInfoLog(prog, sizeof(buf), nullptr, buf);
	printf("link log = [%s]\n", buf);
	glValidateProgram(prog);
	glGetProgramiv(prog, GL_VALIDATE_STATUS, &status);
	printf("prog validate status = %s\n", (status == GL_TRUE) ? "true" : "false");
	glGetProgramInfoLog(prog, sizeof(buf), nullptr, buf);
	printf("validate log = [%s]\n", buf);
	fflush(stdout);
	// execute it!
	glUseProgram(prog);
	// done
	free( (void*)vertSource );
	free( (void*)fragSource );
}

glm::vec4 vertCube[] = { // 12 * 3 = 36 (vertices + color)
	// face 0,1: v0-v3-v2, v0-v2-v1, red
	{ -0.5F, -0.5F, +0.5F, 1.0F }, { 1.0F, 0.3F, 0.3F, 1.0F }, // v0
	{ -0.5F, -0.5F, -0.5F, 1.0F }, { 1.0F, 0.3F, 0.3F, 1.0F }, // v3
	{ +0.5F, -0.5F, -0.5F, 1.0F }, { 1.0F, 0.3F, 0.3F, 1.0F }, // v2
	{ -0.5F, -0.5F, +0.5F, 1.0F }, { 1.0F, 0.3F, 0.3F, 1.0F }, // v0
	{ +0.5F, -0.5F, -0.5F, 1.0F }, { 1.0F, 0.3F, 0.3F, 1.0F }, // v2
	{ +0.5F, -0.5F, +0.5F, 1.0F }, { 1.0F, 0.3F, 0.3F, 1.0F }, // v1
	// face 2,3: v1-v2-v6, v1-v6-v5, blue
	{ +0.5F, -0.5F, +0.5F, 1.0F }, { 0.3F, 0.3F, 1.0F, 1.0F }, // v1
	{ +0.5F, -0.5F, -0.5F, 1.0F }, { 0.3F, 0.3F, 1.0F, 1.0F }, // v2
	{ +0.5F, +0.5F, -0.5F, 1.0F }, { 0.3F, 0.3F, 1.0F, 1.0F }, // v6
	{ +0.5F, -0.5F, +0.5F, 1.0F }, { 0.3F, 0.3F, 1.0F, 1.0F }, // v1
	{ +0.5F, +0.5F, -0.5F, 1.0F }, { 0.3F, 0.3F, 1.0F, 1.0F }, // v6
	{ +0.5F, +0.5F, +0.5F, 1.0F }, { 0.3F, 0.3F, 1.0F, 1.0F }, // v5
	// face 4,5: v2-v3-v7, v2-v7-v6, green
	{ +0.5F, -0.5F, -0.5F, 1.0F }, { 0.3F, 1.0F, 0.3F, 1.0F }, // v2
	{ -0.5F, -0.5F, -0.5F, 1.0F }, { 0.3F, 1.0F, 0.3F, 1.0F }, // v3
	{ -0.5F, +0.5F, -0.5F, 1.0F }, { 0.3F, 1.0F, 0.3F, 1.0F }, // v7
	{ +0.5F, -0.5F, -0.5F, 1.0F }, { 0.3F, 1.0F, 0.3F, 1.0F }, // v2
	{ -0.5F, +0.5F, -0.5F, 1.0F }, { 0.3F, 1.0F, 0.3F, 1.0F }, // v7
	{ +0.5F, +0.5F, -0.5F, 1.0F }, { 0.3F, 1.0F, 0.3F, 1.0F }, // v6
	// face 6,7: v3-v0-v4, v3-v4-v7, cyan
	{ -0.5F, -0.5F, -0.5F, 1.0F }, { 0.3F, 1.0F, 1.0F, 1.0F }, // v3
	{ -0.5F, -0.5F, +0.5F, 1.0F }, { 0.3F, 1.0F, 1.0F, 1.0F }, // v0
	{ -0.5F, +0.5F, +0.5F, 1.0F }, { 0.3F, 1.0F, 1.0F, 1.0F }, // v4
	{ -0.5F, -0.5F, -0.5F, 1.0F }, { 0.3F, 1.0F, 1.0F, 1.0F }, // v3
	{ -0.5F, +0.5F, +0.5F, 1.0F }, { 0.3F, 1.0F, 1.0F, 1.0F }, // v4
	{ -0.5F, +0.5F, -0.5F, 1.0F }, { 0.3F, 1.0F, 1.0F, 1.0F }, // v7
	// face 8,9: v1-v5-v4, v1-v4-v0, magenta
	{ +0.5F, -0.5F, +0.5F, 1.0F }, { 1.0F, 0.3F, 1.0F, 1.0F }, // v1
	{ +0.5F, +0.5F, +0.5F, 1.0F }, { 1.0F, 0.3F, 1.0F, 1.0F }, // v5
	{ -0.5F, +0.5F, +0.5F, 1.0F }, { 1.0F, 0.3F, 1.0F, 1.0F }, // v4
	{ +0.5F, -0.5F, +0.5F, 1.0F }, { 1.0F, 0.3F, 1.0F, 1.0F }, // v1
	{ -0.5F, +0.5F, +0.5F, 1.0F }, { 1.0F, 0.3F, 1.0F, 1.0F }, // v4
	{ -0.5F, -0.5F, +0.5F, 1.0F }, { 1.0F, 0.3F, 1.0F, 1.0F }, // v0
	// face 10,11: v4-v5-v6, v4-v6-v7, yellow
	{ -0.5F, +0.5F, +0.5F, 1.0F }, { 1.0F, 1.0F, 0.3F, 1.0F }, // v4
	{ +0.5F, +0.5F, +0.5F, 1.0F }, { 1.0F, 1.0F, 0.3F, 1.0F }, // v5
	{ +0.5F, +0.5F, -0.5F, 1.0F }, { 1.0F, 1.0F, 0.3F, 1.0F }, // v6
	{ -0.5F, +0.5F, +0.5F, 1.0F }, { 1.0F, 1.0F, 0.3F, 1.0F }, // v4
	{ +0.5F, +0.5F, -0.5F, 1.0F }, { 1.0F, 1.0F, 0.3F, 1.0F }, // v6
	{ -0.5F, +0.5F, -0.5F, 1.0F }, { 1.0F, 1.0F, 0.3F, 1.0F }, // v7
	// your reference
#if 0
	{ -0.5F, -0.5F, -0.5F, 1.0F }, // v0
	{ +0.5F, -0.5F, -0.5F, 1.0F }, // v1
	{ +0.5F, -0.5F, +0.5F, 1.0F }, // v2
	{ -0.5F, -0.5F, +0.5F, 1.0F }, // v3
	{ -0.5F, +0.5F, -0.5F, 1.0F }, // v4
	{ +0.5F, +0.5F, -0.5F, 1.0F }, // v5
	{ +0.5F, +0.5F, +0.5F, 1.0F }, // v6
	{ -0.5F, +0.5F, +0.5F, 1.0F }, // v7
#endif
};

glm::vec4 vertPyramid[] = { // 6 * 3 = 18 (vertices + color)
	// face 0: v0-v1-v2, red
	{ 0.0F, 0.5F, 0.0F, 1.0F }, { 1.0F, 0.3F, 0.3F, 1.0F, }, // v0
	{ 0.5F, -0.3F, 0.0F, 1.0F }, { 1.0F, 0.3F, 0.3F, 1.0F, }, // v1
	{ 0.0F, -0.3F, -0.5F, 1.0F }, { 1.0F, 0.3F, 0.3F, 1.0F, }, // v2
	// face 1: v0-v2-v3, green
	{ 0.0F, 0.5F, 0.0F, 1.0F }, { 0.3F, 1.0F, 0.3F, 1.0F, }, // v0
	{ 0.0F, -0.3F, -0.5F, 1.0F }, { 0.3F, 1.0F, 0.3F, 1.0F, }, // v2
	{ -0.5F, -0.3F, 0.0F, 1.0F }, { 0.3F, 1.0F, 0.3F, 1.0F, }, // v3
	// face 2: v0-v3-v4, blue
	{ 0.0F, 0.5F, 0.0F, 1.0F }, { 0.3F, 0.3F, 1.0F, 1.0F, }, // v0
	{ -0.5F, -0.3F, 0.0F, 1.0F }, { 0.3F, 0.3F, 1.0F, 1.0F, }, // v3
	{ 0.0F, -0.3F, 0.5F, 1.0F }, { 0.3F, 0.3F, 1.0F, 1.0F, }, // v4
	// face 3: v0-v4-v1, yellow
	{ 0.0F, 0.5F, 0.0F, 1.0F }, { 1.0F, 1.0F, 0.3F, 1.0F, }, // v0
	{ 0.0F, -0.3F, 0.5F, 1.0F }, { 1.0F, 1.0F, 0.3F, 1.0F, }, // v4
	{ 0.5F, -0.3F, 0.0F, 1.0F }, { 1.0F, 1.0F, 0.3F, 1.0F, }, // v1
	// face 4: v1-v4-v3, cyan
	{ 0.5F, -0.3F, 0.0F, 1.0F }, { 0.3F, 1.0F, 1.0F, 1.0F, }, // v1
	{ 0.0F, -0.3F, 0.5F, 1.0F }, { 0.3F, 1.0F, 1.0F, 1.0F, }, // v4
	{ -0.5F, -0.3F, 0.0F, 1.0F }, { 0.3F, 1.0F, 1.0F, 1.0F, }, // v3
	// face 5: v1-v3-v2, cyan
	{ 0.5F, -0.3F, 0.0F, 1.0F }, { 0.3F, 1.0F, 1.0F, 1.0F, }, // v1
	{ -0.5F, -0.3F, 0.0F, 1.0F }, { 0.3F, 1.0F, 1.0F, 1.0F, }, // v3
	{ 0.0F, -0.3F, -0.5F, 1.0F }, { 0.3F, 1.0F, 1.0F, 1.0F, }, // v2
	// your reference
#if 0
	{ 0.0F, 0.5F, 0.0F, 1.0F }, // v0
	{ 0.5F, -0.3F, 0.0F, 1.0F }, // v1
	{ 0.0F, -0.3F, 0.5F, 1.0F }, // v2
	{ -0.5F, -0.3F, 0.0F, 1.0F }, // v3
	{ 0.0F, -0.3F, -0.5F, 1.0F }, // v4
#endif
};

glm::mat4 matPyramid = glm::mat4( 1.0F );
glm::mat4 matCube = glm::mat4( 1.0F );
glm::mat4 matView = glm::mat4( 1.0F );
glm::mat4 matProj = glm::mat4( 1.0F );

float theta = 0.0F;
system_clock::time_point lastTime = system_clock::now();

void updateFunc(void) {
	system_clock::time_point curTime = system_clock::now();
	milliseconds elapsedTimeMSEC = duration_cast<milliseconds>(curTime - lastTime); // in millisecond
	theta = (elapsedTimeMSEC.count() / 1000.0F) * (float)M_PI_2; // in <math.h>, M_PI_2 = pi/2
	// model transform for the pyramid
	matPyramid = glm::mat4( 1.0F );
	matPyramid = glm::translate( matPyramid, glm::vec3(-0.4F, 0.0F, 0.0F));
	matPyramid = glm::rotate( matPyramid, 2.0F * theta, glm::vec3(0.0F, 1.0F, 0.0F));
	matPyramid = glm::scale( matPyramid, glm::vec3(0.5F, 0.5F, 0.5F));
	// model transform for the cube
	matCube = glm::mat4( 1.0F );
	matCube = glm::translate( matCube, glm::vec3(0.4F, 0.0F, 0.0F));
	matCube = glm::rotate( matCube, 2.0F * theta, glm::vec3(1.0F, 0.0F, 0.0F));
	matCube = glm::scale( matCube, glm::vec3(0.3F, 0.3F, 0.3F));
	// viewing transform
	const GLfloat radius = 2.0F;
	matView = glm::lookAtRH(
	              glm::vec3( radius * sinf(theta), 20 * 0.05F, radius * cosf(theta) ),
	              glm::vec3( 0.02F, 0.0F, 0.0F ),
	              glm::vec3( 0.0F, 1.0F, 0.0F )
	          );
	// projection matrix
#if 0
	matProj = glm::orthoRH(
	              -1.0F, +1.0F,
	              -0.75F, +0.75F,
	              +1.0F, +3.0F
	          );
#endif
	GLfloat xmin = -1.0F;
	GLfloat xmax = +1.0F;
	GLfloat ymin = -0.75F;
	GLfloat ymax = +0.75F;
	GLfloat znear = +1.0F; // GLfloat zmin = -znear;
	GLfloat zfar = +3.0F; // GLfloat zmax = -zfar;
	matProj[0][0] = 2.0F / (xmax - xmin); matProj[1][0] = 0.0; matProj[2][0] = 0.0;
	matProj[0][1] = 0.0; matProj[1][1] = 2.0F / (ymax - ymin); matProj[2][1] = 0.0;
	matProj[0][2] = 0.0; matProj[1][2] = 0.0; matProj[2][2] = 2.0F / (znear - zfar); // matProj[2][2] = 2.0F / (zmax - zmin);
	matProj[3][0] = -(xmax + xmin) / (xmax - xmin);
	matProj[3][1] = -(ymax + ymin) / (ymax - ymin);
	matProj[3][2] = -(zfar + znear) / (zfar - znear); // matProj[3][2] = -(zmax + zmin) / (zmax - zmin);
	matProj[0][3] = 0.0; matProj[1][3] = 0.0; matProj[2][3] = 0.0; matProj[3][3] = 1.0;
	// print out
#if 1
	static bool first = true;
	if (first == true) {
		std::cout << glm::to_string( matProj ) << std::endl;
		first = false;
	}
#endif
}

void drawFunc(void) {
	glEnable(GL_DEPTH_TEST);
	glDepthRange(0.0F, 1.0F);
	glClearDepthf(1.0F);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	// clear in gray color
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	// matrix settings
	GLuint locView = glGetUniformLocation(prog, "uView");
	glUniformMatrix4fv(locView, 1, GL_FALSE, glm::value_ptr(matView));
	GLuint locProj = glGetUniformLocation(prog, "uProj");
	glUniformMatrix4fv(locProj, 1, GL_FALSE, glm::value_ptr(matProj));
	// draw the pyramid
	GLuint locPos = glGetAttribLocation(prog, "aPos");
	glEnableVertexAttribArray(locPos);
	glVertexAttribPointer(locPos, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), glm::value_ptr(vertPyramid[0]));
	GLuint locColor = glGetAttribLocation(prog, "aColor");
	glEnableVertexAttribArray(locColor);
	glVertexAttribPointer(locColor, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), glm::value_ptr(vertPyramid[1]));
	GLuint locModel = glGetUniformLocation(prog, "uModel");
	glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(matPyramid));
	glDrawArrays(GL_TRIANGLES, 0, 18); // 18 vertices
	// draw the cube
	glVertexAttribPointer(locPos, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), glm::value_ptr(vertCube[0]));
	glVertexAttribPointer(locColor, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), glm::value_ptr(vertCube[1]));
	glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(matCube));
	glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices
	// done
	glFinish();
}

void refreshFunc(GLFWwindow* window) {
	// refresh
	drawFunc();
	// GLFW action
	glfwSwapBuffers(window);
}

void keyFunc(GLFWwindow* window, int key, int scancode, int action, int mods) {
	switch (key) {
	case GLFW_KEY_ESCAPE:
		if (action == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		break;
	case GLFW_KEY_R:
		if (action == GLFW_PRESS) {
			lastTime = system_clock::now();
		}
		break;
	}
}

int main(int argc, char* argv[]) {
	const char* basename = getBaseName( argv[0] );
	// start GLFW & GLEW
	glfwInit();
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
	GLFWwindow* window = glfwCreateWindow(WIN_W, WIN_H, basename, nullptr, nullptr);
	glfwSetWindowPos(window, WIN_X, WIN_Y);
	glfwMakeContextCurrent(window);
	glewInit();
	// prepare
	glfwSetWindowRefreshCallback(window, refreshFunc);
	glfwSetKeyCallback(window, keyFunc);
	glClearColor(0.5F, 0.5F, 0.5F, 1.0F);
	// main loop
	initFunc();
	while (! glfwWindowShouldClose(window)) {
		// animation loop
		updateFunc();
		drawFunc();
		// GLFW actions
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// done
	glfwTerminate();
	return 0;
}
