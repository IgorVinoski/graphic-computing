#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

#include <vector>
#include <Eigen/Dense>
#include "CGMath.hpp"


using namespace std;
using namespace Eigen;

struct Camera
{
    Vector3f pos;

    Vector3f rotX, rotY, rotZ;
    
    float anguloRotX; //pitch, rotação m torno do eixo X (cima/baixo)
    float anguloRotY; //yaw, rotação em torno do eixo Y (esquerda/direita)

    float velT = 0.0505051f;
    float velR = 0.0007f;

    double mousex =0, mousey =0;

    Camera(){}
    Camera(Vector3f camPos, Vector3f camPosRef);
    
};


Matrix4f getMatLookAt(Camera &cam);

void rotacionar(Camera &cam, double x, double y, bool reset = false);


inline void moveFrente(Camera &cam){
    cam.pos -= cam.rotZ * cam.velT;
}
inline void moveTras(Camera &cam){
    cam.pos += cam.rotZ * cam.velT;
}
inline void moveDireita(Camera &cam){
    cam.pos += cam.rotX * cam.velT;
}
inline void moveEsquerda(Camera &cam){
    cam.pos -= cam.rotX * cam.velT;
}
inline void moveCima(Camera &cam){
    cam.pos += cam.rotY * cam.velT;
}
inline void moveBaixo(Camera &cam){
    cam.pos -= cam.rotY * cam.velT;
}

#endif