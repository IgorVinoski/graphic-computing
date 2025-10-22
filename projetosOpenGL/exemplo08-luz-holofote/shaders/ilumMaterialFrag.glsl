#version 330

out vec4 fragCorSaida;

in vec3 fragNormal;      // normal interpolada do fragmento
in vec3 fragPos;     // posição do fragmento no espaço do mundo

struct Luz {
    vec3 posicao;
    vec3 direcao;
    float anguloAbertura;
    float kl, kq; // direção da luz vindo do "sol". Sentido do vetor: "superfície -> sol"
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
uniform bool usarAtenuacao;


void main(void)
{
    vec3 ambiente = vec3(0.0);
    vec3 difusa   = vec3(0.0);
    vec3 especular = vec3(0.0);

    vec3 N = normalize(fragNormal);
    vec3 L = normalize(luz.posicao - fragPos); //L


    float angulo = acos(dot(L, normalize(-luz.direcao)));

	// ambiente
    if(usarLuzAmbiente)
        ambiente = mat.ka * luz.Ia;
  	
    // difusa
    if(usarLuzDifusa && angulo < luz.anguloAbertura)
    {
        float intensidadeDifusa = max(dot(N, L ), 0.0); //(N.L) = cos(theta)
        difusa = mat.kd  * luz.Id * intensidadeDifusa; //d · I · max(N·L, 0)
    }
    
    
    // especular
    if(usarLuzEspecular && angulo < luz.anguloAbertura)
    {
        vec3 V = normalize(posCamera - fragPos);
        vec3 R = reflect(-L, N);
        float anguloVR = max(dot(V, R), 0.0); //(V.R) = cos(theta)  
        float intensidadeEspecular = pow(anguloVR, mat.brilho);
        especular = mat.ks  * luz.Is *  intensidadeEspecular; 
    }
    

    //atenuacao
    if(usarAtenuacao)
    {
        float distancia = length(luz.posicao - fragPos);
        float atenuacao  = 1.0 / (1.0 + luz.kl * distancia + luz.kq * (distancia * distancia)); 
        ambiente  *= atenuacao;  
        difusa   *= atenuacao;
        especular *= atenuacao; 
    }
    

	//cor final
    vec3 corCombinada = ambiente + difusa + especular;
    fragCorSaida = vec4(corCombinada, 1.0);
}