#ifndef __cplusplus
#error This file works only with C++
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <chrono>
#include <iostream>
#include <algorithm>
using namespace std;
using namespace std::chrono;

#pragma warning(disable: 4711 4710 4100 4514 4626 4774 4365 4625 4464 4571 4201 5026 5027 5039)

#define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp> // for glm::to_string()
#include <glm/gtc/type_ptr.hpp> // for glm::value_ptr( )

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glfw3.lib")
#pragma warning(disable: 4711 4710 4100)

#include "./common.c"

const unsigned int WIN_W = 500; // window size in pixels, (Width, Height)
const unsigned int WIN_H = 500;
const unsigned int WIN_X = 100; // window position in pixels, (X, Y)
const unsigned int WIN_Y = 100;

const char* vertFileName = "c42-set-mat4.vert";
const char* fragFileName = "c42-set-mat4.frag";

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

glm::vec4 vertPos[] = { // 6 * 3 = 18 vertices
	// face 0: v0-v1-v2
	{ 0.0F, 0.5F, 0.0F, 1.0F }, // v0
	{ 0.5F, -0.3F, 0.0F, 1.0F }, // v1
	{ 0.0F, -0.3F, -0.5F, 1.0F }, // v2
	// face 1: v0-v2-v3
	{ 0.0F, 0.5F, 0.0F, 1.0F }, // v0
	{ 0.0F, -0.3F, -0.5F, 1.0F }, // v2
	{ -0.5F, -0.3F, 0.0F, 1.0F }, // v3
	// face 2: v0-v3-v4
	{ 0.0F, 0.5F, 0.0F, 1.0F }, // v0
	{ -0.5F, -0.3F, 0.0F, 1.0F }, // v3
	{ 0.0F, -0.3F, 0.5F, 1.0F }, // v4
	// face 3: v0-v4-v1
	{ 0.0F, 0.5F, 0.0F, 1.0F }, // v0
	{ 0.0F, -0.3F, 0.5F, 1.0F }, // v4
	{ 0.5F, -0.3F, 0.0F, 1.0F }, // v1
	// face 4: v1-v4-v3
	{ 0.5F, -0.3F, 0.0F, 1.0F }, // v1
	{ 0.0F, -0.3F, 0.5F, 1.0F }, // v4
	{ -0.5F, -0.3F, 0.0F, 1.0F }, // v3
	// face 5: v1-v3-v2
	{ 0.5F, -0.3F, 0.0F, 1.0F }, // v1
	{ -0.5F, -0.3F, 0.0F, 1.0F }, // v3
	{ 0.0F, -0.3F, -0.5F, 1.0F }, // v2
	// your reference
#if 0
	{ 0.0F, 0.5F, 0.0F, 1.0F }, // v0
	{ 0.5F, -0.3F, 0.0F, 1.0F }, // v1
	{ 0.0F, -0.3F, -0.5F, 1.0F }, // v2
	{ -0.5F, -0.3F, 0.0F, 1.0F }, // v3
	{ 0.0F, -0.3F, 0.5F, 1.0F }, // v4
#endif
};

glm::vec4 vertColor[] = {
	// face 0: red
	{ 1.0F, 0.3F, 0.3F, 1.0F, },
	{ 1.0F, 0.3F, 0.3F, 1.0F, },
	{ 1.0F, 0.3F, 0.3F, 1.0F, },
	// face 1: green
	{ 0.3F, 1.0F, 0.3F, 1.0F, },
	{ 0.3F, 1.0F, 0.3F, 1.0F, },
	{ 0.3F, 1.0F, 0.3F, 1.0F, },
	// face 2: blue
	{ 0.3F, 0.3F, 1.0F, 1.0F, },
	{ 0.3F, 0.3F, 1.0F, 1.0F, },
	{ 0.3F, 0.3F, 1.0F, 1.0F, },
	// face 3: yellow
	{ 1.0F, 1.0F, 0.3F, 1.0F, },
	{ 1.0F, 1.0F, 0.3F, 1.0F, },
	{ 1.0F, 1.0F, 0.3F, 1.0F, },
	// face 4: cyan
	{ 0.3F, 1.0F, 1.0F, 1.0F, },
	{ 0.3F, 1.0F, 1.0F, 1.0F, },
	{ 0.3F, 1.0F, 1.0F, 1.0F, },
	// face 5: cyan
	{ 0.3F, 1.0F, 1.0F, 1.0F, },
	{ 0.3F, 1.0F, 1.0F, 1.0F, },
	{ 0.3F, 1.0F, 1.0F, 1.0F, },
};

glm::mat4 mat = glm::mat4( 1.0F ); // mat = matDrag * matModel
glm::mat4 matDrag = glm::mat4( 1.0F );
glm::mat4 matUpdated = glm::mat4( 1.0F );

void keyFunc(GLFWwindow* window, int key, int scancode, int action, int mods) {
	switch (key) {
	case GLFW_KEY_ESCAPE:
		if (action == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		break;
	case GLFW_KEY_R:
		mat = matDrag = matUpdated = glm::mat4( 1.0F ); // reset every thing
		break;
	}
}

int mousePressed = GL_FALSE;
glm::vec2 dragStart; // mouse dragging start point

glm::vec3 calcUnitVec(const glm::vec2& raw) {
	glm::vec2 scr;
	// debug for out of window conditions
#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201703L) || __cplusplus >= 201703L) // C++17 or later
	scr.x = std::clamp(raw.x, 0.0F, (float)WIN_W);
	scr.y = std::clamp(raw.y, 0.0F, (float)WIN_H);
#else
	scr.x = (org.x < 0.0F ? 0.0F : (org.x > WIN_W ? WIN_W : org.x));
	scr.y = (org.y < 0.0F ? 0.0F : (org.y > WIN_H ? WIN_H : org.y));
#endif
	// normal processing
	const GLfloat radius = sqrtf(WIN_W * WIN_W + WIN_H * WIN_H) / 2.0F;
	glm::vec3 v;
	v.x = (scr.x - WIN_W / 2.0F) / radius;
	v.y = (WIN_H / 2.0F - scr.y) / radius;
	v.z = sqrtf(1.0F - v.x * v.x - v.y * v.y);
	return v;
}

void matMult(GLfloat c[16], GLfloat a[16], GLfloat b[16]) { // C = A * B
	c[0] = a[0] * b[0] + a[4] * b[1] + a[8] * b[2] + a[12] * b[3];
	c[1] = a[1] * b[0] + a[5] * b[1] + a[9] * b[2] + a[13] * b[3];
	c[2] = a[2] * b[0] + a[6] * b[1] + a[10] * b[2] + a[14] * b[3];
	c[3] = a[3] * b[0] + a[7] * b[1] + a[11] * b[2] + a[15] * b[3];
	//
	c[4] = a[0] * b[4] + a[4] * b[5] + a[8] * b[6] + a[12] * b[7];
	c[5] = a[1] * b[4] + a[5] * b[5] + a[9] * b[6] + a[13] * b[7];
	c[6] = a[2] * b[4] + a[6] * b[5] + a[10] * b[6] + a[14] * b[7];
	c[7] = a[3] * b[4] + a[7] * b[5] + a[11] * b[6] + a[15] * b[7];
	//
	c[8] = a[0] * b[8] + a[4] * b[9] + a[8] * b[10] + a[12] * b[11];
	c[9] = a[1] * b[8] + a[5] * b[9] + a[9] * b[10] + a[13] * b[11];
	c[10] = a[2] * b[8] + a[6] * b[9] + a[10] * b[10] + a[14] * b[11];
	c[11] = a[3] * b[8] + a[7] * b[9] + a[11] * b[10] + a[15] * b[11];
	//
	c[12] = a[0] * b[12] + a[4] * b[13] + a[8] * b[14] + a[12] * b[15];
	c[13] = a[1] * b[12] + a[5] * b[13] + a[9] * b[14] + a[13] * b[15];
	c[14] = a[2] * b[12] + a[6] * b[13] + a[10] * b[14] + a[14] * b[15];
	c[15] = a[3] * b[12] + a[7] * b[13] + a[11] * b[14] + a[15] * b[15];
}

GLfloat matRot[16] = {
	1.0, 0.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.0, 0.0, 0.0, 1.0,
};
GLfloat matA[16];
GLfloat matB[16];

void my_trackball(GLfloat u[3], GLfloat v[3]) {
	GLfloat n[3];
	GLfloat costheta, sintheta, theta;	
	GLfloat q[4]; // quaternion (x, y, z, w)
	n[0] = u[1] * v[2] - u[2] * v[1];
	n[1] = u[2] * v[0] - u[0] * v[2];
	n[2] = u[0] * v[1] - u[1] * v[0];
	costheta = u[0] * v[0] + u[1] * v[1] + u[2] * v[2];
	sintheta = sqrtf(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]); // it is also the length of vector n
	theta = atan2f(sintheta, costheta);
	// code corrected: vector n should be normalized
	n[0] /= sintheta;
	n[1] /= sintheta;
	n[2] /= sintheta;
	// END code corrected: vector n should be normalized
	q[0] = n[0] * sinf(theta / 2.0F);
	q[1] = n[1] * sinf(theta / 2.0F);
	q[2] = n[2] * sinf(theta / 2.0F);
	q[3] = cosf(theta / 2.0F);
	matA[0] =  q[3]; matA[4] =  q[2]; matA[8]  = -q[1]; matA[12] =  q[0];
	matA[1] = -q[2]; matA[5] =  q[3]; matA[9]  =  q[0]; matA[13] =  q[1];
	matA[2] =  q[1]; matA[6] = -q[0]; matA[10] =  q[3]; matA[14] =  q[2];
	matA[3] = -q[0]; matA[7] = -q[1]; matA[11] = -q[2]; matA[15] =  q[3];
	matB[0] =  q[3]; matB[4] =  q[2]; matB[8]  = -q[1]; matB[12] = -q[0];
	matB[1] = -q[2]; matB[5] =  q[3]; matB[9]  =  q[0]; matB[13] = -q[1];
	matB[2] =  q[1]; matB[6] = -q[0]; matB[10] =  q[3]; matB[14] = -q[2];
	matB[3] =  q[0]; matB[7] =  q[1]; matB[11] =  q[2]; matB[15] =  q[3];
	matMult(matRot, matA, matB);
}

glm::mat4 calcTrackball(const glm::vec2& start, const glm::vec2& cur) {
	glm::vec3 org = calcUnitVec( start );
	glm::vec3 dst = calcUnitVec( cur );
	my_trackball( glm::value_ptr(org), glm::value_ptr(dst) );
	glm::mat4 m = glm::mat4(
		matRot[0], matRot[4], matRot[8], matRot[12], 
		matRot[1], matRot[5], matRot[9], matRot[13], 
		matRot[2], matRot[6], matRot[10], matRot[14], 
		matRot[3], matRot[7], matRot[11], matRot[15] 
	);
	return m;
}

void cursorEnterFunc(GLFWwindow* win, int entered) {
	printf("cursor %s the window\n", (entered == GL_FALSE) ? "leaving" : "entering");
	fflush(stdout);
}

void cursorPosFunc(GLFWwindow* win, double xscr, double yscr) {
	if (mousePressed == GL_TRUE) {
		glm::vec2 dragCur = glm::vec2((GLfloat)xscr, (GLfloat)yscr);
		matDrag = calcTrackball( dragStart, dragCur );
		mat = matDrag * matUpdated;
	}
}

void mouseButtonFunc(GLFWwindow* win, int button, int action, int mods) {
	GLdouble x, y;
	switch (action) {
	case GLFW_PRESS:
		mousePressed = GL_TRUE;
		glfwGetCursorPos(win, &x, &y);
		dragStart = glm::vec2((GLfloat)x, (GLfloat)y);
		break;
	case GLFW_RELEASE:
		mousePressed = GL_FALSE;
		glfwGetCursorPos(win, &x, &y);
		glm::vec2 dragCur = glm::vec2((GLfloat)x, (GLfloat)y);
		matDrag = calcTrackball( dragStart, dragCur );
		mat = matDrag * matUpdated;
		matDrag = glm::mat4(1.0F); // reset
		matUpdated = mat; // update to the object matrix
		break;
	}
	fflush(stdout);
}

void updateFunc(void) {
	// do nothing
}

void drawFunc(void) {
	// z-buffer setting
	glEnable(GL_DEPTH_TEST);
	glDepthRange(0.0F, 1.0F);
	glClearDepthf(1.0F);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	// clear in gray color
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	// provide the vertex attributes
	GLuint locPos = glGetAttribLocation(prog, "aPos");
	glEnableVertexAttribArray(locPos);
	glVertexAttribPointer(locPos, 4, GL_FLOAT, GL_FALSE, 0, glm::value_ptr(vertPos[0]));
	GLuint locColor = glGetAttribLocation(prog, "aColor");
	glEnableVertexAttribArray(locColor);
	glVertexAttribPointer(locColor, 4, GL_FLOAT, GL_FALSE, 0, glm::value_ptr(vertColor[0]));
	GLuint locMat = glGetUniformLocation(prog, "uMat");
	glUniformMatrix4fv(locMat, 1, GL_FALSE, glm::value_ptr(mat));
	// draw the pyramid
	glDrawArrays(GL_TRIANGLES, 0, 18); // 18 vertices
	// done
	glFinish();
}

void refreshFunc(GLFWwindow* window) {
	// refresh
	drawFunc();
	// GLFW action
	glfwSwapBuffers(window);
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
	glfwSetCursorEnterCallback(window, cursorEnterFunc);
	glfwSetCursorPosCallback(window, cursorPosFunc);
	glfwSetMouseButtonCallback(window, mouseButtonFunc);
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
