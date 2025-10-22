#version 330

layout (location=0) in vec3 coordVert;


uniform mat4 matPV; //projection x view
uniform mat4 matModel; // model


void main(void)
{
	gl_Position = matPV * matModel * vec4(coordVert, 1.0); //multiplica as matrizes pela coordenada homogênea
}