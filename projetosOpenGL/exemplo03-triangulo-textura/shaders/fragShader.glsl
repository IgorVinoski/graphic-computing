#version 330

out vec4 FragColor;
in vec2 txCoord; //mesmo nome da variável definina no vertex shader
uniform sampler2D texture0; //(binding=0) -> GL_TEXTURE0

void main(void)
{
	FragColor = texture(texture0, txCoord);	
}


