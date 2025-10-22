#include "CGMath.hpp"


float degree2rad(float g) {
    float r = g * (float)(_PI / 180.0f);
    return r;
}


Matrix4f getMatProj(int largJan, int altJan, float vFOV, float zNear, float zFar)
{
     //computa a matriz de projeção
	 float aspectRatio = float(largJan) / float(altJan);
     vFOV = degree2rad(60.0f);
	 float halfFOV = tan(vFOV/2.0f);
	 
	 
	 Matrix4f matProj = Matrix4f::Zero();
	 matProj(0,0) = 1.0f / (halfFOV * aspectRatio);
	 matProj(1,1) = 1.0f / halfFOV;
	 matProj(2,2) = (zNear+zFar) / (-zFar + zNear);
	 matProj(2,3) = (2.0f * zNear * zFar) / (-zFar + zNear);
	 matProj(3,2) = -1.0f;

     return matProj;
}