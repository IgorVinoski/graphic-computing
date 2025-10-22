#version 330

out vec4 fragCorSaida;
in vec4 fragCor; //mesmo nome da variável definina no vertex shader

void main(void)
{
	fragCorSaida = fragCor;
}