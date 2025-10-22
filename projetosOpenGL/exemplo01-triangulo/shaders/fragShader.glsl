#version 330
out vec4 FragColor;

uniform vec3 corFrag;

void main(void)
{	
	FragColor = vec4(corFrag, 1.0);
}

