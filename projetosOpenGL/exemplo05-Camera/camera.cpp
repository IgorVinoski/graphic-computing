#include "camera.hpp"


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

void rotacionar(Camera &cam, double x, double y)
{

    //##Variável static dentro da função:
    //      -A variável só é criada uma única vez, na primeira vez que a função é chamada.
    //      -Ela mantém seu valor entre as chamadas subsequentes.
    //      -Ela não é destruída quando a função termina — ela vive até o fim do programa.
    static double xVelho = x;
    static double yVelho = y;

    //calcula o deslocamento nas coordenadas X e Y do mouse
    double dx = x - xVelho;
    double dy = y - yVelho;
    
    //atualiza as variaveis para a proxima chamada da funcao (a variavel estática guarda os valores, como uma variavel global)
    xVelho = x;
    yVelho = y;

    //cout << dx << " - " << dy << endl;

    double aux = cam.anguloRotX + (dy * cam.velR);
    if(abs(aux) < _PI_2) //limitar a rotação em X a 90 graus
        cam.anguloRotX += (dy * cam.velR);


    cam.anguloRotY += dx * cam.velR;

    //converter para cartesiano navamente e atualizar o vetor Z com base nos angulos alterados 
    Vector3f z = esferico_2_cartesiano(cam.anguloRotX, cam.anguloRotY);

    cam.rotZ = z; 
    cam.rotZ.normalize();

    cam.rotX = Vector3f(0,1,0).cross(cam.rotZ); 
    cam.rotX.normalize();

    cam.rotY= cam.rotZ.cross(cam.rotX);
    cam.rotY.normalize();
}