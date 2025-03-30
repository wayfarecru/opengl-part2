#version 330

in vec4 aPos;
in vec4 aNorm;

out vec4 vColor;

uniform mat4 uMat;

uniform vec4 light_position;
uniform vec4 light_ambient;
uniform vec4 light_diffuse;
uniform vec4 light_specular;
uniform vec4 light_att; // for (a + b*d + c*d^2)

uniform vec4 material_ambient;
uniform vec4 material_diffuse;
uniform vec4 material_specular;
uniform float material_shineness;

void main(void) {
	vec4 vPos = uMat * aPos; // position in view frame
	vec4 vNorm = uMat * aNorm; // normal vector in view frame
	vec3 N = normalize(vNorm.xyz);
	vec3 L = normalize(light_position.xyz - vPos.xyz);
	vec3 V = vec3(0.0F, 0.0F, 1.0F);
	vec3 R = normalize(2 * dot(L, N) * N - L);
	vec4 ambient = light_ambient * material_ambient;
	float d = length(light_position.xyz - vPos.xyz);
	float denom = light_att.x + light_att.y * d + light_att.z * d * d;
	vec4 diffuse = max(dot(L, N), 0.0) * light_diffuse * material_diffuse / denom;
	vec4 specular = pow(max(dot(R, V), 0.0), material_shineness) * light_specular * material_specular / denom;
	vColor = ambient + diffuse + specular;
	gl_Position = vPos;
}

