#include "camera.hpp"
#include <iostream>


Camera::Camera(Vector3f camPos, Vector3f camPosRef)
{
    //define a posicao da camera
    pos = camPos;
    //computa a rotação
    rotZ = camPos - camPosRef;
    rotZ.normalize();

    rotX = Eigen::Vector3f(0,1,0).cross(rotZ); 
    rotX.normalize();

    rotY = rotZ.cross(rotX);
    rotY.normalize();

    //extrair os angulos phi e theta do vetor Z
    auto [anguloRotX, anguloRotY] =  cartesiano_2_esferico(rotZ);

}

Matrix4f getMatLookAt(Camera &cam)
{

    //translacao inversa
    Matrix4f translation = getMatTranslacao(-cam.pos);
    Matrix4f rotation = Matrix4f::Identity();

    rotation.row(0).head(3) = cam.rotX;
    rotation.row(1).head(3) = cam.rotY;
    rotation.row(2).head(3) = cam.rotZ;

    return (rotation * translation);
}

void rotacionar(Camera &cam, double x, double y, bool reset)
{

    //##Variável static dentro da função:
    //      -A variável só é criada uma única vez, na primeira vez que a função é chamada.
    //      -Ela mantém seu valor entre as chamadas subsequentes.
    //      -Ela não é destruída quando a função termina — ela vive até o fim do programa.
    //static double xVelho = x;
    //static double yVelho = y;

    //calcula o deslocamento nas coordenadas X e Y do mouse
    double dx = x - cam.mousex;
    double dy = y - cam.mousey;

    
    if(abs(dx)>150 || abs(dy)>150 || reset)
    {
        //cout << dx << " - " << dy <<  " - " << endl;
        cam.mousex = x;
        cam.mousey = y;
        return;
    }
    
    //atualiza as variaveis para a proxima chamada da funcao (a variavel estática guarda os valores, como uma variavel global)
    cam.mousex = x;
    cam.mousey = y;



    cam.anguloRotX += dy * cam.velR;

    // limitar em ±(PI/2 - margem)
    float limite = _PI_2 - 0.01f; //a margem é muito importante, evitar cos(Pi/2)=0!!!
    if (cam.anguloRotX > limite) cam.anguloRotX = limite;
    if (cam.anguloRotX < -limite) cam.anguloRotX = -limite;


    cam.anguloRotY -= dx * cam.velR;


    //converter para cartesiano navamente e atualizar o vetor Z com base nos angulos alterados 
    Vector3f z = esferico_2_cartesiano(cam.anguloRotX, cam.anguloRotY);

    cam.rotZ = z; 
    cam.rotZ.normalize();

    cam.rotX = Vector3f(0,1,0).cross(cam.rotZ); 
    cam.rotX.normalize();

    cam.rotY= cam.rotZ.cross(cam.rotX);
    cam.rotY.normalize();
}