#version 330

layout (location=0) in vec3 coordVert;
layout (location=3) in vec3 normVert;


uniform mat4 matPV; //projection x view
uniform mat4 matModel; // model

flat out vec3 fragNormal; 
out vec3 fragPos;


void main(void)
{
	vec4 pos = matModel * vec4(coordVert, 1.0);
	fragPos = vec3(pos);
	mat3 matNormal = mat3(transpose(inverse(matModel)));
    fragNormal = matNormal * normVert;
	gl_Position = matPV * pos; 
}
