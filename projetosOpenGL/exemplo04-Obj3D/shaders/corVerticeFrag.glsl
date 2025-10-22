#version 330

out vec4 FragColor;
in vec4 vCor; //mesmo nome da variável definina no vertex shader

void main(void)
{
	FragColor = vCor;
}