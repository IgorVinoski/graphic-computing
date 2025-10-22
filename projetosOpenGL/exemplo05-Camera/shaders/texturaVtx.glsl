#version 330

layout (location=0) in vec3 coordVert;
layout (location=2) in vec2 coordTexture;

uniform mat4 mat;

out vec2 vTex;

void main(void)
{
	gl_Position = mat * vec4(coordVert, 1.0); //multiplica as matrizes pela coordenada homogênea
	vTex = coordTexture;
} 