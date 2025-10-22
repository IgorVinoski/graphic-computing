#version 330

layout (location=0) in vec3 coordVert;
layout (location=2) in vec2 coordTexture;
layout (location=3) in vec3 normVert;

uniform mat4 matPV; //projection x view
uniform mat4 matModel; // model

out vec3 fragNormal; 
out vec3 fragPos;
out vec2 fragTex;

void main(void)
{
	fragTex = coordTexture;
	vec4 pos = matModel * vec4(coordVert, 1.0);
	fragPos = vec3(pos);
	mat3 matNormal = mat3(transpose(inverse(matModel)));
    fragNormal = matNormal * normVert;
	gl_Position = matPV * pos; 
} 