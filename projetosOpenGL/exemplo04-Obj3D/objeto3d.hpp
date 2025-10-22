#ifndef OBJETO_H_INCLUDED
#define OBJETO_H_INCLUDED

#include <Eigen/Dense>
#include <glad/glad.h> //GLAD
#include <vector>
#include <string>
#include "shader.hpp"

using namespace std;
using namespace Eigen;


struct Objeto3D
{
    GLuint VAO;
    GLuint VBO_POS;
    GLuint VBO_COR;
    GLuint VBO_TEXTURA;
    GLuint VBO_NORMAL;
    size_t nVertices;
    Vector4f cor;
    Vector3f posicao;
    GLuint texturaPtr;
    string nome;


    Objeto3D()
    {
        VAO = 0;
        VBO_POS = 0;
        VBO_COR = 0;
        nVertices = 0;
        cor = Vector4f(1.0f, 0.0f, 0.0f, 1.0f);
        posicao = Vector3f(0.0f, 0.0f, 0.0f); //origem
        VBO_TEXTURA = 0;
        VBO_NORMAL = 0;
        texturaPtr = 0;
        nome = "";
    }

};

//OBJ
bool carregarOBJ(vector<Objeto3D> &objetos, string caminhoArq, string nome);


//PLY
bool carregarPLY(vector<Objeto3D> &objetos, string caminhoArq, string nome);


//posicao + normais (opcional)
bool criarObjPos(Objeto3D &obj, string nome, vector<Vector3f> coordVertices, vector<Vector3f> normais={});

//posicao + cor + normais (opcional)
bool criarObjPosCor(Objeto3D &obj, string nome, vector<Vector3f> coordVertices, vector<Vector4f> corVertices, vector<Vector3f> normais={});

//posicao + textura + normais (opcional)
bool criarObjPosTex(Objeto3D &obj, string nome, vector<Vector3f> coordVertices, vector<Vector2f> coordTextura, string arqTextura, vector<Vector3f> normais={});

void renderizarObjeto(Objeto3D &obj);

void excluirObjeto(Objeto3D &obj);

ShaderTipo definirShader(const Objeto3D& obj);

Vector3f calcularCentroBoundingBox(const vector<Vector3f>& pontos);


#endif