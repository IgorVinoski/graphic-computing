#ifndef ILUMINACAO_H_INCLUDED
#define ILUMINACAO_H_INCLUDED

#include <Eigen/Dense>

//using namespace std;
using namespace Eigen;


enum MAT_TIPO {
    MAT_LATAO,
    MAT_BRONZE,
    MAT_BRONZE_POLIDO,
    MAT_CROMO,
    MAT_COBRE,
    MAT_COBRE_POLIDO,
    MAT_OURO,
    MAT_OURO_POLIDO,
    MAT_ESTANHO,
    MAT_PRATA,
    MAT_PRATA_POLIDA,
    MAT_ESMERALDA,
    MAT_JADE,
    MAT_OBSIDIANA,
    MAT_PEROLA,
    MAT_RUBI,
    MAT_TURQUESA,
    MAT_PLASTICO_PRETO,
    MAT_PLASTICO_CIANO,
    MAT_PLASTICO_VERDE,
    MAT_PLASTICO_VERMELHO,
    MAT_PLASTICO_BRANCO,
    MAT_PLASTICO_AMARELO,
    MAT_BORRACHA_PRETA,
    MAT_BORRACHA_CIANO,
    MAT_BORRACHA_VERDE,
    MAT_BORRACHA_VERMELHA,
    MAT_BORRACHA_BRANCA,
    MAT_BORRACHA_AMARELA,
    MAT_PADRAO
};


struct LuzDirecional
{
    Vector3f Ia;
    Vector3f Id;
    Vector3f Is;
    Vector3f direcao;;
    LuzDirecional();
};

struct LuzPontual
{
    Vector3f Ia;
    Vector3f Id;
    Vector3f Is;
    Vector3f posicao;
    float kl, kq;
    LuzPontual();
};


struct LuzHolofote
{
    Vector3f Ia;
    Vector3f Id;
    Vector3f Is;
    Vector3f posicao;
    Vector3f direcao;
    float anguloAbertura;
    float kl, kq;
    LuzHolofote();
};

struct Material
{
    Vector3f ka; //ka
    Vector3f kd; //kd 
    Vector3f ks; //ks
    float brilho; //Ns

    Material();
    Material(Vector3f KD, float perKa=0.15f, float perKd=0.7f, float bri=10.0f);
    Material(MAT_TIPO tipo);
};

Vector3f normalizar(Vector3f v, float vMax);










#endif
