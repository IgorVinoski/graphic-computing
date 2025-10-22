#version 330
out vec4 fragCorSaida;

in vec3 fragNormal;      // normal interpolada do fragmento
in vec3 fragPos;     // posição do fragmento no espaço do mundo
in vec2 fragTex; //coordenada de textura interpolada do fragmento
uniform sampler2D texture0; //-> GL_TEXTURE0 -> difusa
uniform sampler2D texture1; //-> GL_TEXTURE1 -> especular
uniform bool usarTexturaDifusa;

struct Luz {
    vec3 direcao;
    vec3 Ia, Id, Is;
};

struct Material {
	vec3 ka, kd, ks;
	float brilho;
}; 

uniform vec3 posCamera;
uniform Material mat;
uniform Luz luz;

uniform bool usarLuzAmbiente;
uniform bool usarLuzDifusa;
uniform bool usarLuzEspecular;


void main(void)
{
	vec3 ambiente = vec3(0.0);
    vec3 difusa   = vec3(0.0);
    vec3 especular = vec3(0.0);

	vec3 kd = texture(texture0, fragTex).rgb;
	vec3 ka = vec3(mat.ka.x); //usando o primeiro valor de ka como intensidade da luz ambiente
	ka = ka * kd; 

	vec3 ks = vec3(mat.ks.x);//usando o primeiro valor de ks como intensidade da luz especular 
	

	if(usarTexturaDifusa)
		ks = texture(texture1, fragTex).rgb;
	


    vec3 N = normalize(fragNormal);
    vec3 L = normalize(-luz.direcao); //"superfície -> sol, precisamos inverter o sentido

	// ambiente
    if(usarLuzAmbiente)
		ambiente = ka * luz.Ia;
        
  	
    // difusa
    if(usarLuzDifusa)
    {
        float intensidadeDifusa = max(dot(N, L ), 0.0); //(N.L) = cos(theta)
        difusa = kd  * luz.Id * intensidadeDifusa; //d · I · max(N·L, 0)
    }
    
    
    // especular
    if(usarLuzEspecular)
    {
        vec3 V = normalize(posCamera - fragPos);
        vec3 R = reflect(-L, N);
        float anguloVR = max(dot(V, R), 0.0); //(V.R) = cos(theta)  
        float intensidadeEspecular = pow(anguloVR, mat.brilho);
        especular = ks  * luz.Is *  intensidadeEspecular; 
    }
    

	//cor final
    vec3 corCombinada = ambiente + difusa + especular;
    fragCorSaida = vec4(corCombinada, 1.0);
}

