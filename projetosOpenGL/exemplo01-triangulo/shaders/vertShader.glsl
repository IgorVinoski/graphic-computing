#version 330

layout (location=0) in vec3 coordVert;

uniform mat4 mat;

void main(void)
{
	gl_Position = mat * vec4(coordVert, 1.0); //multiplica as matrizes pela coordenada homogênea
} 
