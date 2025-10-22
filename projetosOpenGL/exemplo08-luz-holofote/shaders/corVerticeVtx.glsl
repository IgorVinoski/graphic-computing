#version 330

layout (location=0) in vec3 coordVert;
layout (location=1) in vec4 corVert;

uniform mat4 matPV; //projection x view
uniform mat4 matModel; // model

out vec4 fragCor;

void main(void)
{
	gl_Position = matPV * matModel * vec4(coordVert, 1.0); //multiplica as matrizes pela coordenada homogênea
	fragCor = corVert;
} 