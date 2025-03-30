#version 330

in vec4 aPos;
in vec4 aNorm;
out vec4 vPos;
out vec4 vNorm;

uniform mat4 uMat;

void main(void) {
	vPos = uMat * aPos; // position in view frame 
	vNorm = transpose(inverse(uMat)) * aNorm; // normal vector in view frame
	gl_Position = vPos;
}

