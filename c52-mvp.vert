#version 330 core

in vec4 aPos; // vertex position: attribute
in vec4 aColor; // vertex color: attribute
out vec4 vColor; // varying color: varying
uniform mat4 uModel; // model matrix: uniform
uniform mat4 uView; // view matrix: uniform
uniform mat4 uProj; // projection matrix: uniform

void main(void) {
	gl_Position = uProj * uView * uModel * aPos; // transformation
	vColor = aColor;
}
