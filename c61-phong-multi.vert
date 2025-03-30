#version 330

in vec4 aPos;
in vec4 aNorm;
out vec4 vColor;

uniform mat4 uMat;

struct Light {
	vec4 pos;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 att;
};
uniform Light light[2];

struct Material {
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shineness;
};
uniform Material material;

void main(void) {
	vec4 vPos = uMat * aPos; // position in view frame
	vec4 vNorm = uMat * aNorm; // normal vector in view frame
	vec3 N = normalize(vNorm.xyz);
	vec3 V = vec3(0.0F, 0.0F, 1.0F);
	vColor = vec4(0.0, 0.0, 0.0, 0.0);
	for (int i = 0; i < 2; ++i) {
		vec3 L = normalize(light[i].pos.xyz - vPos.xyz);
		vec3 R = reflect(-L, N);
		vec4 ambient = light[i].ambient * material.ambient;
		float d = length(light[i].pos.xyz - vPos.xyz);
		float denom = light[i].att.x + light[i].att.y * d + light[i].att.z * d * d;
		vec4 diffuse = max(dot(L, N), 0.0) * light[i].diffuse * material.diffuse / denom;
		vec4 specular = pow(max(dot(R, V), 0.0), material.shineness) * light[i].specular * material.specular / denom;
		vColor += ambient + diffuse + specular;
	}
	gl_Position = vPos;
}

