#include "iluminacao.hpp"
#include "CGMath.hpp"


LuzDirecional::LuzDirecional()
{

    Ia = Vector3f(1.0f, 1.0f, 1.0f); 
    Id = Vector3f(1.0f, 1.0f, 1.0f);
    Is = Vector3f(1.0f, 1.0f, 1.0f);
    direcao = Vector3f(0.0f, -1.0f, 0.0f);
}

LuzPontual::LuzPontual()
{

    Ia = Vector3f(1.0f, 1.0f, 1.0f); 
    Id = Vector3f(1.0f, 1.0f, 1.0f);
    Is = Vector3f(1.0f, 1.0f, 1.0f);
    posicao = Vector3f(0.0f, 0.0f, 0.0f);
    kl = 0.3f;
    kq = 0.1f;

}

LuzHolofote::LuzHolofote()
{
    Ia = Vector3f(1.0f, 1.0f, 1.0f); 
    Id = Vector3f(1.0f, 1.0f, 1.0f);
    Is = Vector3f(1.0f, 1.0f, 1.0f);
    posicao = Vector3f(0.0f, 0.0f, 0.0f);
    direcao = Vector3f(0.0f, -1.0f, 0.0f);
    anguloAbertura = graus2rad(12.0f);
    kl = 0.3f;
    kq = 0.1f;
}


Material::Material()
{
    ka = Vector3f(0.15f, 0.15f, 0.15f);
    kd = Vector3f(1.0f, 1.0f, 1.0f);
    ks = Vector3f(0.3f, 0.3f, 0.3f);
    brilho = 8.0f;
}

Material::Material(Vector3f KD, float perKa, float perKd, float bri)
{
    kd = KD;

    if(perKa<0 || perKa>1.0f)
        perKa=0.1f;
    if(perKd<0 || perKd>1.0f)
        perKd=0.7f;

    ka  = normalizar(kd, perKa);
    ks = normalizar(kd, perKd);
    brilho = bri;
}

Material::Material(MAT_TIPO tipo)
{
    if (tipo == MAT_LATAO) {
        ka = Vector3f(0.329412f, 0.223529f, 0.027451f);
        kd = Vector3f(0.780392f, 0.568627f, 0.113725f);
        ks = Vector3f(0.992157f, 0.941176f, 0.807843f);
        brilho = 27.8974f;
    }
    else if (tipo == MAT_BRONZE) {
        ka = Vector3f(0.2125f, 0.1275f, 0.054f);
        kd = Vector3f(0.714f, 0.4284f, 0.18144f);
        ks = Vector3f(0.393548f, 0.271906f, 0.166721f);
        brilho = 25.6f;
    }
    else if (tipo == MAT_BRONZE_POLIDO) {
        ka = Vector3f(0.25f, 0.148f, 0.06475f);
        kd = Vector3f(0.4f, 0.2368f, 0.1036f);
        ks = Vector3f(0.774597f, 0.458561f, 0.200621f);
        brilho = 76.8f;
    }
    else if (tipo == MAT_CROMO) {
        ka = Vector3f(0.25f, 0.25f, 0.25f);
        kd = Vector3f(0.4f, 0.4f, 0.4f);
        ks = Vector3f(0.774597f, 0.774597f, 0.774597f);
        brilho = 76.8f;
    }
    else if (tipo == MAT_COBRE) {
        ka = Vector3f(0.19125f, 0.0735f, 0.0225f);
        kd = Vector3f(0.7038f, 0.27048f, 0.0828f);
        ks = Vector3f(0.256777f, 0.137622f, 0.086014f);
        brilho = 12.8f;
    }
    else if (tipo == MAT_COBRE_POLIDO) {
        ka = Vector3f(0.2295f, 0.08825f, 0.0275f);
        kd = Vector3f(0.5508f, 0.2118f, 0.066f);
        ks = Vector3f(0.580594f, 0.223257f, 0.0695701f);
        brilho = 51.2f;
    }
    else if (tipo == MAT_OURO) {
        ka = Vector3f(0.24725f, 0.1995f, 0.0745f);
        kd = Vector3f(0.75164f, 0.60648f, 0.22648f);
        ks = Vector3f(0.628281f, 0.555802f, 0.366065f);
        brilho = 51.2f;
    }
    else if (tipo == MAT_OURO_POLIDO) {
        ka = Vector3f(0.24725f, 0.2245f, 0.0645f);
        kd = Vector3f(0.34615f, 0.3143f, 0.0903f);
        ks = Vector3f(0.797357f, 0.723991f, 0.208006f);
        brilho = 83.2f;
    }
    else if (tipo == MAT_ESTANHO) {
        ka = Vector3f(0.105882f, 0.058824f, 0.113725f);
        kd = Vector3f(0.427451f, 0.470588f, 0.541176f);
        ks = Vector3f(0.333333f, 0.333333f, 0.521569f);
        brilho = 9.84615f;
    }
    else if (tipo == MAT_PRATA) {
        ka = Vector3f(0.19225f, 0.19225f, 0.19225f);
        kd = Vector3f(0.50754f, 0.50754f, 0.50754f);
        ks = Vector3f(0.508273f, 0.508273f, 0.508273f);
        brilho = 51.2f;
    }
    else if (tipo == MAT_PRATA_POLIDA) {
        ka = Vector3f(0.23125f, 0.23125f, 0.23125f);
        kd = Vector3f(0.2775f, 0.2775f, 0.2775f);
        ks = Vector3f(0.773911f, 0.773911f, 0.773911f);
        brilho = 89.6f;
    }
    else if (tipo == MAT_ESMERALDA) {
        ka = Vector3f(0.0215f, 0.1745f, 0.0215f);
        kd = Vector3f(0.07568f, 0.61424f, 0.07568f);
        ks = Vector3f(0.633f, 0.727811f, 0.633f);
        brilho = 76.8f;
    }
    else if (tipo == MAT_JADE) {
        ka = Vector3f(0.135f, 0.2225f, 0.1575f);
        kd = Vector3f(0.54f, 0.89f, 0.63f);
        ks = Vector3f(0.316228f, 0.316228f, 0.316228f);
        brilho = 12.8f;
    }
    else if (tipo == MAT_OBSIDIANA) {
        ka = Vector3f(0.05375f, 0.05f, 0.06625f);
        kd = Vector3f(0.18275f, 0.17f, 0.22525f);
        ks = Vector3f(0.332741f, 0.328634f, 0.346435f);
        brilho = 38.4f;
    }
    else if (tipo == MAT_PEROLA) {
        ka = Vector3f(0.25f, 0.20725f, 0.20725f);
        kd = Vector3f(1.0f, 0.829f, 0.829f);
        ks = Vector3f(0.296648f, 0.296648f, 0.296648f);
        brilho = 11.264f;
    }
    else if (tipo == MAT_RUBI) {
        ka = Vector3f(0.1745f, 0.01175f, 0.01175f);
        kd = Vector3f(0.61424f, 0.04136f, 0.04136f);
        ks = Vector3f(0.727811f, 0.626959f, 0.626959f);
        brilho = 76.8f;
    }
    else if (tipo == MAT_TURQUESA) {
        ka = Vector3f(0.1f, 0.18725f, 0.1745f);
        kd = Vector3f(0.396f, 0.74151f, 0.69102f);
        ks = Vector3f(0.297254f, 0.30829f, 0.306678f);
        brilho = 12.8f;
    }
    else if (tipo == MAT_PLASTICO_PRETO) {
        ka = Vector3f(0.0f, 0.0f, 0.0f);
        kd = Vector3f(0.01f, 0.01f, 0.01f);
        ks = Vector3f(0.5f, 0.5f, 0.5f);
        brilho = 32.0f;
    }
    else if (tipo == MAT_PLASTICO_CIANO) {
        ka = Vector3f(0.0f, 0.1f, 0.06f);
        kd = Vector3f(0.0f, 0.50980392f, 0.50980392f);
        ks = Vector3f(0.50196078f, 0.50196078f, 0.50196078f);
        brilho = 32.0f;
    }
    else if (tipo == MAT_PLASTICO_VERDE) {
        ka = Vector3f(0.0f, 0.0f, 0.0f);
        kd = Vector3f(0.1f, 0.35f, 0.1f);
        ks = Vector3f(0.45f, 0.55f, 0.45f);
        brilho = 32.0f;
    }
    else if (tipo == MAT_PLASTICO_VERMELHO) {
        ka = Vector3f(0.0f, 0.0f, 0.0f);
        kd = Vector3f(0.5f, 0.0f, 0.0f);
        ks = Vector3f(0.7f, 0.6f, 0.6f);
        brilho = 32.0f;
    }
    else if (tipo == MAT_PLASTICO_BRANCO) {
        ka = Vector3f(0.0f, 0.0f, 0.0f);
        kd = Vector3f(0.55f, 0.55f, 0.55f);
        ks = Vector3f(0.7f, 0.7f, 0.7f);
        brilho = 32.0f;
    }
    else if (tipo == MAT_PLASTICO_AMARELO) {
        ka = Vector3f(0.0f, 0.0f, 0.0f);
        kd = Vector3f(0.5f, 0.5f, 0.0f);
        ks = Vector3f(0.6f, 0.6f, 0.5f);
        brilho = 32.0f;
    }
    else if (tipo == MAT_BORRACHA_PRETA) {
        ka = Vector3f(0.02f, 0.02f, 0.02f);
        kd = Vector3f(0.01f, 0.01f, 0.01f);
        ks = Vector3f(0.4f, 0.4f, 0.4f);
        brilho = 10.0f;
    }
    else if (tipo == MAT_BORRACHA_CIANO) {
        ka = Vector3f(0.0f, 0.05f, 0.05f);
        kd = Vector3f(0.4f, 0.5f, 0.5f);
        ks = Vector3f(0.04f, 0.7f, 0.7f);
        brilho = 10.0f;
    }
    else if (tipo == MAT_BORRACHA_VERDE) {
        ka = Vector3f(0.0f, 0.05f, 0.0f);
        kd = Vector3f(0.4f, 0.5f, 0.4f);
        ks = Vector3f(0.04f, 0.7f, 0.04f);
        brilho = 10.0f;
    }
    else if (tipo == MAT_BORRACHA_VERMELHA) {
        ka = Vector3f(0.05f, 0.0f, 0.0f);
        kd = Vector3f(0.5f, 0.4f, 0.4f);
        ks = Vector3f(0.7f, 0.04f, 0.04f);
        brilho = 10.0f;
    }
    else if (tipo == MAT_BORRACHA_BRANCA) {
        ka = Vector3f(0.05f, 0.05f, 0.05f);
        kd = Vector3f(0.5f, 0.5f, 0.5f);
        ks = Vector3f(0.7f, 0.7f, 0.7f);
        brilho = 10.0f;
    }
    else if (tipo == MAT_BORRACHA_AMARELA) {
        ka = Vector3f(0.05f, 0.05f, 0.0f);
        kd = Vector3f(0.5f, 0.5f, 0.4f);
        ks = Vector3f(0.7f, 0.7f, 0.04f);
        brilho = 10.0f;
    }
    else if(tipo == MAT_PADRAO)
    {
        ka = Vector3f(0.15f, 0.15f, 0.15f);
        kd = Vector3f(1.0f, 1.0f, 1.0f);
        ks = Vector3f(0.3f, 0.3f, 0.3f);
        brilho = 8.0f;
    }

}



Vector3f normalizar(Vector3f v, float vMax)
{

    float maxx = v.maxCoeff();
    float escala = vMax / maxx;
    return v * escala;
}



