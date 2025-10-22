#version 330

layout (location=0) in vec3 coordVert;
layout (location=1) in vec3 corVert;

uniform mat4 mat;

out vec3 cor;

void main(void)
{
	gl_Position = mat * vec4(coordVert, 1.0); //multiplica as matrizes pela coordenada homogênea
	cor = corVert;
} 
