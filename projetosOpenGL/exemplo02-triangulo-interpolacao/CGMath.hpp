#ifndef CG_MATH_H_INCLUDED
#define CG_MATH_H_INCLUDED

#include <Eigen/Dense>
using namespace Eigen;

#define _PI		3.14159265358979323846
#define _PI_2	1.57079632679489661923
#define _PI_4	0.78539816339744830962


float degree2rad(float g);

Matrix4f getMatProj(int largJan, int altJan, float vFOV, float zNear, float zFar);


#endif