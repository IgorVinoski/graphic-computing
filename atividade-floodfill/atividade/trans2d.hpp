#ifndef TRANS2D_H_INCLUDED
#define TRANS2D_H_INCLUDED
#include <Eigen/Dense>
#include <cmath>

using namespace Eigen;
#define __PI__ 3.14159265359

float graus2rad(float g)
{
    return ((g*__PI__)/180.0f);

}
Matrix3f getCisalhamentoHorizontal(float a){
    Matrix3f M = Matrix3f::Identity();
    M(0,1) = a;
   

    return M;
}
Matrix3f getCisalhamentoVertical(float a){
    Matrix3f M = Matrix3f::Identity();
    M(1,0) = a;
   

    return M;
}
Matrix3f getReflexaoY() {
    Matrix3f M = Matrix3f::Identity();
    M(0,0) = -1;
    M(1,1) = 1;
    M(2,2) = 1;
   

    return M;
}
Matrix3f getReflexaoH() {
    Matrix3f M = Matrix3f::Identity();
    M(0,0) = 1;
    M(1,1) = -1;
    M(2,2) = 1;
   

    return M;
}

Matrix3f getTranslacao(float tx, float ty)
{
    Matrix3f M = Matrix3f::Identity();
    M(0,2) = tx;
    M(1,2) = ty;

    return M;
}

Matrix3f getEscala(float sx, float sy)
{
    Matrix3f M = Matrix3f::Identity();
    M(0,0) =sx;
    M(1,1) = sy;

    return M;
}

Matrix3f getRotacao(float g) //graus
{
    float s = sin(graus2rad(g));
    float c = cos(graus2rad(g));
    Matrix3f M = Matrix3f::Identity();
    M(0,0) =c;
    M(0,1) = -s;
    M(1,0) =s;
    M(1,1) = c;

    return M;
}

#endif