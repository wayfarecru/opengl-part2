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
uniform Light light;

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
	vec3 L = normalize(light.pos.xyz - vPos.xyz);
	vec3 V = vec3(0.0F, 0.0F, 1.0F);
	vec3 R = reflect(-L, N);
	vec4 ambient = light.ambient * material.ambient;
	float d = length(light.pos.xyz - vPos.xyz);
	float denom = light.att.x + light.att.y * d + light.att.z * d * d;
	vec4 diffuse = max(dot(L, N), 0.0) * light.diffuse * material.diffuse / denom;
	vec4 specular = pow(max(dot(R, V), 0.0), material.shineness) * light.specular * material.specular / denom;
	vColor = ambient + diffuse + specular;
	gl_Position = vPos;
}

