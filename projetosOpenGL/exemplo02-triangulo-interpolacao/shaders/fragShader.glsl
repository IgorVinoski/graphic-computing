#version 330
out vec4 FragColor;

in vec3 cor;

void main(void)
{	
	FragColor = vec4(cor, 1.0);
}

