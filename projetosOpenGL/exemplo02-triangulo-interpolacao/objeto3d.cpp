#include <iostream>
#include "objeto3d.hpp"
#include "erro.hpp"

using namespace std;
using namespace Eigen;

//Para configurar os atributos dos vértices, cada variável de entrada no vertexshader está associada ao metadado "location"
#define VERTEX_ATTRIB_POS 0 //no vertexshader, a variável de entrada que define a posição do vértice possui "layout (location=0)" 
#define VERTEX_ATTRIB_COR 1 //no vertexshader, a variável de entrada que define a cor do vértice possui "layout (location=1)" 

void excluirObjeto(Objeto3D& obj)
{

       //remove buffer
       if(obj.VBO_POS)
       glDeleteBuffers(1, &(obj.VBO_POS));
   
     //remove buffer
     if(obj.VBO_COR)
         glDeleteBuffers(1, &(obj.VBO_COR));
     
 
 
 
     //remove o VAO
     if(obj.VAO)
         glDeleteVertexArrays(1, &(obj.VAO));
         
     obj.VAO = 0;
     obj.VBO_POS = 0;
     obj.VBO_COR = 0;
     obj.nVertices = 0;
     obj.nome = "";
     obj.cor = Vector4f(1.0f, 0.0f, 0.0f, 1.0f);
     obj.posicao = Vector3f(0.0f, 0.0f, 0.0f);

}



bool criarObjPosCor(Objeto3D &obj, string nome, vector<Vector3f> coordVertices, vector<Vector4f> corVertices)
{
    
    //verificação de erros com base nos parâmetros 
    if(coordVertices.size()<3)
    {
        ERRO(); //Erro, o objeto deve possuir pelo menos 3 vértices!;
        return false;
    }

    if(coordVertices.size()%3 != 0)
    {
        ERRO(); //Erro, o número de vértices deve ser múltiplos de 3, pois triângulos serão renderizados!";
        return false;
    }

    if( corVertices.size() != coordVertices.size())
    {
        ERRO(); //Erro, os arrays devem possuir o mesmo tamanho.";
        return false;
    }


    //se um objeto já foi criado, destroi ele primeiro
    excluirObjeto(obj);

    obj.nVertices = coordVertices.size();
    obj.nome = nome;
    obj.posicao = calcularCentroBoundingBox(coordVertices);


    //Cria o Vertex Array Object (VAO). ELe vai armazenar as informações dos atributos dos vértices
    glGenVertexArrays(1, &(obj.VAO)); //cria um ponteiro para o VAO
    glBindVertexArray(obj.VAO); //bind

    //Cria o Vertex Array Object (VAO). ELe vai armazenar as informações dos atributos dos vértices
    //glGenVertexArrays(1, &(obj.VAO)); //cria um ponteiro para o VAO
    //glBindVertexArray(obj.VAO); //bind

    //-----------------
    //Para CADA atributo -> criar um buffer (VBO) e carregar os dados do atributo para a memória da GPU. 
    //Em seguida, definir como estes dados serão acessados no vertex shader, associando o buffer criado ao metadado "location" do vertex shader!
    //-----------------

    //-----------------  Posição
    glGenBuffers(1, &(obj.VBO_POS)); //cria um ponteiro para o buffer
    glBindBuffer(GL_ARRAY_BUFFER, obj.VBO_POS); //bind 
    glBufferData(GL_ARRAY_BUFFER, coordVertices.size() * sizeof(Vector3f), &coordVertices[0], GL_STATIC_DRAW); // carra os dados para a GPU
    glEnableVertexAttribArray(VERTEX_ATTRIB_POS);  //Associa o location=0 para o atributo posição do vértice.
    glVertexAttribPointer(VERTEX_ATTRIB_POS, 3,	GL_FLOAT, GL_FALSE,	0, (void*)0);
    //-----------------


    //----------------- Cor
    glGenBuffers(1, &(obj.VBO_COR)); //cria um ponteiro para o buffer
    glBindBuffer(GL_ARRAY_BUFFER, obj.VBO_COR); //bind 
    glBufferData(GL_ARRAY_BUFFER, corVertices.size() * sizeof(Vector4f), &corVertices.at(0), GL_STATIC_DRAW); // carra os dados para a GPU
    glEnableVertexAttribArray(VERTEX_ATTRIB_COR);  
    glVertexAttribPointer(VERTEX_ATTRIB_COR, 4,	GL_FLOAT, GL_FALSE,	0, (void*)0);
    //-----------------


    //unbind all !!
    glBindVertexArray(0); 
    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return true;

} //fim 


bool criarObjPos(Objeto3D &obj, string nome, vector<Vector3f> coordVertices)
{
    
    //verificação de erros com base nos parâmetros 
    if(coordVertices.size()<3)
    {
        ERRO(); //Erro, o objeto deve possuir pelo menos 3 vértices!;
        return false;
    }

    if(coordVertices.size()%3 != 0)
    {
        ERRO(); //Erro, o número de vértices deve ser múltiplos de 3, pois triângulos serão renderizados!";
        return false;
    }


    //se um objeto já foi criado, destroi ele primeiro
    excluirObjeto(obj);

    obj.nVertices = coordVertices.size();
    obj.nome = nome;
    obj.posicao = calcularCentroBoundingBox(coordVertices);


    //Cria o Vertex Array Object (VAO). ELe vai armazenar as informações dos atributos dos vértices
    glGenVertexArrays(1, &(obj.VAO)); //cria um ponteiro para o VAO
    glBindVertexArray(obj.VAO); //bind

    //Cria o Vertex Array Object (VAO). ELe vai armazenar as informações dos atributos dos vértices
    glGenVertexArrays(1, &(obj.VAO)); //cria um ponteiro para o VAO
    glBindVertexArray(obj.VAO); //bind

    //-----------------
    //Para CADA atributo -> criar um buffer (VBO) e carregar os dados do atributo para a memória da GPU. 
    //Em seguida, definir como estes dados serão acessados no vertex shader, associando o buffer criado ao metadado "location" do vertex shader!
    //-----------------

    //-----------------  Posição
    glGenBuffers(1, &(obj.VBO_POS)); //cria um ponteiro para o buffer
    glBindBuffer(GL_ARRAY_BUFFER, obj.VBO_POS); //bind 
    glBufferData(GL_ARRAY_BUFFER, coordVertices.size() * sizeof(Vector3f), &coordVertices[0], GL_STATIC_DRAW); // carra os dados para a GPU
    glEnableVertexAttribArray(VERTEX_ATTRIB_POS);  //Associa o location=0 para o atributo posição do vértice.
    glVertexAttribPointer(VERTEX_ATTRIB_POS, 3,	GL_FLOAT, GL_FALSE,	0, (void*)0);


    //unbind all !!
    glBindVertexArray(0); 
    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return true;

} //fim 


//renderiza um objeto
void renderizarObjeto(Objeto3D &obj)
{
    if(!obj.VAO || obj.nVertices<=0)
        return;



    //Ativa o VAO e os atributos dos vértices associados a este VAO
    glBindVertexArray(obj.VAO);

    //renderizar os triangulos
    glDrawArrays(GL_TRIANGLES, 0, (GLsizei) obj.nVertices);

    //desativa o VAO
    glBindVertexArray(0);



}



Vector3f calcularCentroBoundingBox(const vector<Vector3f>& pontos) {
    // Inicializa com valores extremos
    float minX = numeric_limits<float>::max();
    float minY = numeric_limits<float>::max();
    float minZ = numeric_limits<float>::max();
    float maxX = numeric_limits<float>::lowest();
    float maxY = numeric_limits<float>::lowest();
    float maxZ = numeric_limits<float>::lowest();

    for (const auto& p : pontos) {
        if (p.x() < minX) minX = p.x();
        if (p.y() < minY) minY = p.y();
        if (p.z() < minZ) minZ = p.z();
        if (p.x() > maxX) maxX = p.x();
        if (p.y() > maxY) maxY = p.y();
        if (p.z() > maxZ) maxZ = p.z();
    }

    return Vector3f(
        (minX + maxX) / 2.0f,
        (minY + maxY) / 2.0f,
        (minZ + maxZ) / 2.0f
    );
}


