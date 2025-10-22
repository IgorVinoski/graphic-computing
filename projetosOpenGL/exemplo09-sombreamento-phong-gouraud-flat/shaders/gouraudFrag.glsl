#version 330

out vec4 fragCorSaida;

in vec3 fragCor;

void main(void)
{
    fragCorSaida = vec4(fragCor, 1.0);
}