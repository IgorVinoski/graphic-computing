#ifndef CG_MATH_H_INCLUDED
#define CG_MATH_H_INCLUDED

#include <Eigen/Dense>
using namespace Eigen;
using namespace std;

#define _PI		3.14159265358979323846
#define _PI_2	1.57079632679489661923
#define _PI_4	0.78539816339744830962


float degree2rad(float g);

Vector3f esferico_2_cartesiano(float phi, float theta);

pair<float, float> cartesiano_2_esferico(Vector3f v);

Matrix4f getMatProj(int largJan, int altJan, float vFOV, float zNear, float zFar);

Matrix4f getMatLookAt(Vector3f camPos, Vector3f camAlvo);

Matrix4f getMatTranslacao(Vector3f fator);

Matrix4f getMatEscala(Vector3f fator);

Matrix4f getMatRotacaoX(float angRad);

Matrix4f getMatRotacaoY(float angRad);

Matrix4f getMatRotacaoZ(float angRad);


#endif