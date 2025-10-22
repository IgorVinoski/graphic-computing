#version 330

out vec4 fragCorSaida;
in vec2 fragTex; //mesmo nome da variável definina no vertex shader
uniform sampler2D textura0; //-> GL_TEXTURE0

void main(void)
{
	fragCorSaida = texture(textura0, fragTex);
}