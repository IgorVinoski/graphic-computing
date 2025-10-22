#version 330

layout (location=0) in vec3 coordVert;
layout (location=3) in vec3 normVert;

struct Luz {
    vec3 direcao; // direção da luz vindo do "sol". Sentido do vetor: "superfície -> sol"
    vec3 Ia, Id, Is;
};

struct Material {
	vec3 ka, kd, ks;
	float brilho;
}; 


out vec3 fragCor;

uniform mat4 matPV; //projection x view
uniform mat4 matModel; // model
uniform vec3 posCamera;
uniform Material mat;
uniform Luz luz;

uniform bool usarLuzAmbiente;
uniform bool usarLuzDifusa;
uniform bool usarLuzEspecular;


void main(void)
{
	vec4 pos = matModel * vec4(coordVert, 1.0);

	mat3 matNormal = mat3(transpose(inverse(matModel)));
    vec3 normal = matNormal * normVert;
	gl_Position = matPV * pos; 


	
	vec3 ambiente = vec3(0.0);
    vec3 difusa   = vec3(0.0);
    vec3 especular = vec3(0.0);


    vec3 N = normalize(normal);
    vec3 L = normalize(-luz.direcao); //"superfície -> sol, precisamos inverter o sentido"
        
    

	// ambiente
    if(usarLuzAmbiente)
        ambiente = mat.ka * luz.Ia;
  	
    // difusa
    if(usarLuzDifusa)
    {
        float intensidadeDifusa = max(dot(N, L ), 0.0); //(N.L) = cos(theta)
        difusa = mat.kd  * luz.Id * intensidadeDifusa; //d · I · max(N·L, 0)
    }
    
    
    // especular
    if(usarLuzEspecular)
    {
        vec3 V = normalize(posCamera - vec3(pos));
        vec3 R = reflect(-L, N);
        float anguloVR = max(dot(V, R), 0.0); //(V.R) = cos(theta)  
        float intensidadeEspecular = pow(anguloVR, mat.brilho);
        especular = mat.ks  * luz.Is *  intensidadeEspecular; 
    }
    

	//cor final
    fragCor = ambiente + difusa + especular;

}
