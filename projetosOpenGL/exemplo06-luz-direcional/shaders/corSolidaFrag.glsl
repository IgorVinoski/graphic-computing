#version 330

out vec4 fragCorSaida;
uniform vec3 cor;

void main(void)
{
	fragCorSaida = vec4(cor, 1.0f);
}