#version 330 core

in vec4 aPos; // vertex position: attribute
in vec4 aColor; // vertex color: attribute
out vec4 vColor; // varying color: varying
uniform mat4 uModel; // model matrix: uniform
uniform mat4 uView; // view matrix: uniform

void main(void) {
	gl_Position = uView * uModel * aPos; // transformation
	gl_Position.z *= -1.0F; // negation
	vColor = aColor;
}
