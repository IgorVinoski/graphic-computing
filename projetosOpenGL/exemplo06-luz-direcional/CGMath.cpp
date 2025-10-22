#include "CGMath.hpp"
#include <utility> 
using namespace std;


float graus2rad(float g) {
    float r = g * _PI / 180.0f;
    return r;
}

float rad2graus(float r) {
    float g = r * 180.0f / _PI;
    return g;
}



Vector3f esferico_2_cartesiano(float phi, float theta) 
{
    Vector3f v;
    v(2) = cos(phi) * cos(theta);
    v(0) = cos(phi) * sin(theta);
    v(1) = sin(phi);

    return v;
}

pair<float, float> cartesiano_2_esferico(Vector3f v)
{
    float theta = atan2(v[0], v[2]);
    float phi = asinf(v[1]);
    return {phi, theta};
}


Matrix4f getMatProj(int largJan, int altJan, float vFOV, float zNear, float zFar)
{
     //computa a matriz de projeção
	 float aspectRatio = float(largJan) / float(altJan);
     vFOV = graus2rad(60.0f);
	 float halfFOV = tan(vFOV/2.0f);
	 
	 
	 Matrix4f matProj = Matrix4f::Zero();
	 matProj(0,0) = 1.0f / (halfFOV * aspectRatio);
	 matProj(1,1) = 1.0f / halfFOV;
	 matProj(2,2) = (zNear+zFar) / (-zFar + zNear);
	 matProj(2,3) = (2.0f * zNear * zFar) / (-zFar + zNear);
	 matProj(3,2) = -1.0f;

     return matProj;
}

Matrix4f getMatLookAt(Vector3f camPos, Vector3f camAlvo)
{
	 //computa a rotação
	 Vector3f rZ = camPos - camAlvo;
	 rZ.normalize();
	
	 Vector3f rX = Eigen::Vector3f(0,1,0).cross(rZ); 
	 rX.normalize();
	
	 Vector3f rY = rZ.cross(rX);
	 rY.normalize();

	//translacao inversa
	Matrix4f translacao = getMatTranslacao(-camPos);
	Matrix4f rotacao = Matrix4f::Identity();

	rotacao.row(0).head(3) = rX;
	rotacao.row(1).head(3) = rY;
	rotacao.row(2).head(3) = rZ;

	return (rotacao * translacao);
}


Matrix4f getMatTranslacao(Vector3f fator){

    Matrix4f mat = Matrix4f::Identity();
    mat(0,3) = fator.x();
    mat(1,3) = fator.y();
    mat(2,3) = fator.z();

    return mat;
}


Matrix4f getMatEscala(Vector3f fator)
{
	Matrix4f mat = Matrix4f::Identity();
	mat(0,0) = fator.x();
	mat(1,1) = fator.y();
	mat(2,2) = fator.z();

	return mat;
}

Matrix4f getMatRotacaoX(float angRad)
{
	Matrix4f mat = Matrix4f::Identity();

	float s = sin(angRad);
	float c = cos(angRad);

	mat(1,1) = c;
	mat(1,2) = -s;

	mat(2,1) = s;
	mat(2,2) = c;

	return mat;
}

Matrix4f getMatRotacaoY(float angRad)
{
	Matrix4f mat = Matrix4f::Identity();

	float s = sin(angRad);
	float c = cos(angRad);

	mat(0,0) = c;
	mat(0,2) = s;

	mat(2,0) = -s;
	mat(2,2) = c;

	return mat;
}

Matrix4f getMatRotacaoZ(float angRad)
{
	Matrix4f mat = Matrix4f::Identity();

	float s = sin(angRad);
	float c = cos(angRad);

	mat(0,0) = c;
	mat(0,1) = -s;

	mat(1,0) = s;
	mat(1,1) = c;

	return mat;

}


float normalizarValores(float val, float minVal, float maxVal, float novoMin, float novoMax)
{
	float percentage = (val-minVal) / (maxVal - minVal);
	float novoVal = percentage * (novoMax - novoMin) + novoMin;

	return novoVal;
}