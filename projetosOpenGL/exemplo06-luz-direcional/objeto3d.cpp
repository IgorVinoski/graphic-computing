#include <iostream>
#include "objeto3d.hpp"
#include "textura.hpp"
#include "erro.hpp"
#include "CGMath.hpp"



#include <tiny_obj_loader/tiny_obj_loader.h>

#include <tinyply/tinyply.h>
using namespace tinyply;


#include <filesystem> // C++17
namespace fs = std::filesystem;

#include <unordered_map>



//Para configurar os atributos dos vértices, cada variável de entrada no vertexshader está associada ao metadado "location"
#define VERTEX_ATTRIB_POS 0 //no vertexshader, a variável de entrada que define a posição do vértice possui "layout (location=0)" 
#define VERTEX_ATTRIB_COR 1 //no vertexshader, a variável de entrada que define a cor do vértice possui "layout (location=1)" 
#define VERTEX_ATTRIB_TEX 2 //no vertexshader, a variável de entrada que define a coord de textura do vértice possui "layout (location=2)" 
#define VERTEX_ATTRIB_NORMAL 3



struct Malha //struct usada apenas para a leitura de arquivos .OBJ
{
    vector<Vector3f> vertice = {};
    vector<Vector2f> textura = {};
    vector<Vector3f> normais = {};
    string texturaKd = "";
    string texturaKs = "";
    Material material;
};


void excluirObjeto(Objeto3D& obj)
{

    //remove buffer
    if(obj.VBO_POS)
        glDeleteBuffers(1, &(obj.VBO_POS));
    
    //remove buffer
    if(obj.VBO_COR)
        glDeleteBuffers(1, &(obj.VBO_COR));
    
    //remove buffer
    if(obj.VBO_TEXTURA)
         glDeleteBuffers(1, &(obj.VBO_TEXTURA));

    //remove buffer
    if(obj.VBO_NORMAL)
         glDeleteBuffers(1, &(obj.VBO_NORMAL));

    //remove o VAO
    if(obj.VAO)
        glDeleteVertexArrays(1, &(obj.VAO));
        
     //remove a textura
     if(obj.texturaPtrKd > 0)
        glDeleteTextures(1, &(obj.texturaPtrKd));
    
    if(obj.texturaPtrKs > 0)
        glDeleteTextures(1, &(obj.texturaPtrKs));

    obj.VAO = 0;
    obj.VBO_POS = 0;
    obj.VBO_COR = 0;
    obj.VBO_TEXTURA = 0;
    obj.texturaPtrKd = 0;
    obj.texturaPtrKs = 0;
    obj.VBO_NORMAL = 0;
    obj.nVertices = 0;
    obj.nome = "";
    obj.posicao = Vector3f(0.0f, 0.0f, 0.0f);
}



bool carregarOBJ(vector<Objeto3D> &objetos, string caminhoArq, string nome)
{

    fs::path base_dir = fs::path(caminhoArq).parent_path();

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, caminhoArq.c_str(), base_dir.string().c_str()); 


    if (!ret) {
        ERRO();
        return false;
    }

    // Mapeia material_id para índice da malha correspondente
    std::unordered_map<int, Malha> malhasPorMaterial;

    for (const auto& shape : shapes) {
        size_t index_offset = 0;
        for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) {
            int material_id = shape.mesh.material_ids[f];
            int fv = shape.mesh.num_face_vertices[f];

            // Cria a malha se ainda não existir
            if (malhasPorMaterial.find(material_id) == malhasPorMaterial.end()) {
                Malha m;
                if (material_id >= 0 && material_id < materials.size()) {
                    m.texturaKd = materials[material_id].diffuse_texname;
                    m.texturaKs = materials[material_id].specular_texname;
                    if(m.texturaKd.size()>3)
                    {
                        m.texturaKd = (fs::absolute(base_dir) / m.texturaKd).string(); 
                    }
                    if(m.texturaKs.size()>3)
                    {
                        m.texturaKs = (fs::absolute(base_dir) / m.texturaKs).string(); 
                    }
                    m.material.kd = Vector3f(
                        materials[material_id].diffuse[0],
                        materials[material_id].diffuse[1],
                        materials[material_id].diffuse[2]);
                    m.material.ka = Vector3f(
                        materials[material_id].ambient[0],
                        materials[material_id].ambient[1],
                        materials[material_id].ambient[2]);
                    m.material.ks = Vector3f(
                        materials[material_id].specular[0],
                        materials[material_id].specular[1],
                        materials[material_id].specular[2]);
                    m.material.brilho = materials[material_id].shininess;


                    if(m.material.ka.maxCoeff()>=0.9 || m.material.brilho>=150.0f) //verifica se o material foi definido aleatoriamente pelo blender.
                        m.material = Material(m.material.kd);
                        
                }

                
                malhasPorMaterial[material_id] = m;
            }

            auto& malha = malhasPorMaterial[material_id];

            // Lê os vértices da face
            for (size_t v = 0; v < fv; v++) {
                tinyobj::index_t idx = shape.mesh.indices[index_offset + v];

                // Posição
                Vector3f pos(
                    attrib.vertices[3 * idx.vertex_index + 0],
                    attrib.vertices[3 * idx.vertex_index + 1],
                    attrib.vertices[3 * idx.vertex_index + 2]);
                malha.vertice.push_back(pos);

                // Textura
                if (idx.texcoord_index >= 0) {
                    Vector2f tex(
                        attrib.texcoords[2 * idx.texcoord_index + 0],
                        attrib.texcoords[2 * idx.texcoord_index + 1]);
                    
                    malha.textura.push_back(tex);
                }

                // Normal
                if (idx.normal_index >= 0) {
                    Vector3f norm(
                        attrib.normals[3 * idx.normal_index + 0],
                        attrib.normals[3 * idx.normal_index + 1],
                        attrib.normals[3 * idx.normal_index + 2]);
            
                    malha.normais.push_back(norm);
                }
            }

            index_offset += fv;
        }
    }

    //cria os objetos com os dados da(s) malha(s) carregados
    for (auto& [_, m] : malhasPorMaterial) {
        Objeto3D ob;

        
        if(m.texturaKd.size()>3) //verifica se o arquivo de textura foi definido
            criarObjPosTex(ob, nome, m.vertice, m.textura, m.texturaKd, m.texturaKs, m.normais);
        else
            criarObjPos(ob, nome, m.vertice, m.material, m.normais);
        
        objetos.push_back(move(ob));
    }

    return true;
}


std::shared_ptr<tinyply::PlyData> carregarPropriedadePLY(tinyply::PlyFile& arquivo,
                                                  const std::string& nome_elemento,
                                                  const std::vector<std::string>& propriedades)
{
    try {
        return arquivo.request_properties_from_element(nome_elemento, propriedades);
    }
    catch (const std::exception&) {
        return nullptr;
    }
}

bool carregarPLY(vector<Objeto3D> &objetos, string caminhoArq, string nome)
{
   

    std::ifstream arquivo(caminhoArq, std::ios::binary);
    if (!arquivo.is_open()) {
        ERRO();
        return false;
    }


    tinyply::PlyFile arquivoPLY;
    arquivoPLY.parse_header(arquivo);

    std::shared_ptr<tinyply::PlyData> vertices, normais, cores, faces, tripstrip;

    // vertices = arquivoPLY.request_properties_from_element("vertex", { "x", "y", "z" });
    // normais = arquivoPLY.request_properties_from_element("vertex", { "nx", "ny", "nz" });
    // cores = arquivoPLY.request_properties_from_element("vertex", { "red", "green", "blue", "alpha" });
    // faces = arquivoPLY.request_properties_from_element("face", { "vertex_indices" });

    vertices = carregarPropriedadePLY(arquivoPLY, "vertex", { "x", "y", "z" });
    normais  = carregarPropriedadePLY(arquivoPLY, "vertex", { "nx", "ny", "nz" });
    cores    = carregarPropriedadePLY(arquivoPLY, "vertex", { "red", "green", "blue", "alpha" });
    faces    = carregarPropriedadePLY(arquivoPLY, "face",   { "vertex_indices" });

   
    arquivoPLY.read(arquivo);

    if (!vertices || vertices->count<=0) {
        ERRO(); //erro ao ler as coordenadas dos vértices
        return false;
    }

    if (!faces || faces->count<=0) {
        ERRO(); //erro ao ler as coordenadas dos vértices
        return false;
    }

    int verticesPorFace = faces->buffer.size_bytes() / (faces->count * sizeof(uint32_t));
    if(verticesPorFace != 3)
    {
        ERRO(); // a face não é um triânuglo! exportar o .PLY apenas com triângulos
        return false;
    }

    //struc para ler os dados do PLY
    struct float3 { float x, y, z;};
    struct float4 { float x, y, z, w; };
    struct uint8_t4 { uint8_t x, y, z, w; };

    vector<float3> vf3Pos(vertices->count);
    vector<float3> vf3Norm;
    vector<uint8_t4> vf3Cor;
    vector<Vector3f> vVecPos(faces->count*3);
    vector<Vector3f> vVecNorm = {};
    vector<Vector4f> vVecCor = {};

    vector<uint32_t> dadosFace(faces->count * 3);
    memcpy(dadosFace.data(), faces->buffer.get(), faces->buffer.size_bytes());
    
    
    //vertices
    memcpy(vf3Pos.data(), vertices->buffer.get(), vertices->buffer.size_bytes());

    if (normais && normais->count>0)
    {
        vf3Norm.resize(normais->count); 
        memcpy(vf3Norm.data(), normais->buffer.get(), normais->buffer.size_bytes());
        vVecNorm.resize(faces->count*3); 
    }

    if (cores && cores->count>0)
    {
        vf3Cor.resize(cores->count); 
        memcpy(vf3Cor.data(), cores->buffer.get(), cores->buffer.size_bytes());
        vVecCor.resize(faces->count*3);
    }

    

    // cout << " vf3Pos: "<< vf3Pos.size() << endl;
    // cout << " vf3Norm: "<< vf3Norm.size() << endl;
    // cout << " vf3Cor: "<< vf3Cor.size() << endl;
    // cout << " faces->count: "<< faces->count << endl;

    
    size_t idx=0;
    for (size_t i = 0; i < faces->count; i++)
    {
        
        int idx0 = dadosFace[idx];
        int idx1 = dadosFace[idx+1];
        int idx2 = dadosFace[idx+2];

        // cout << idx0 << ", " << idx1 << ", " << idx2 << "\n";

        vVecPos[idx]   = Vector3f(vf3Pos[idx0].x, vf3Pos[idx0].y, vf3Pos[idx0].z);
        vVecPos[idx+1] = Vector3f(vf3Pos[idx1].x, vf3Pos[idx1].y, vf3Pos[idx1].z);
        vVecPos[idx+2] = Vector3f(vf3Pos[idx2].x, vf3Pos[idx2].y, vf3Pos[idx2].z);
        if (normais && normais->count>0)
        {
            vVecNorm[idx]   = Vector3f(vf3Norm[idx0].x, vf3Norm[idx0].y, vf3Norm[idx0].z);
            vVecNorm[idx+1] = Vector3f(vf3Norm[idx1].x, vf3Norm[idx1].y, vf3Norm[idx1].z);
            vVecNorm[idx+2] = Vector3f(vf3Norm[idx2].x, vf3Norm[idx2].y, vf3Norm[idx2].z);
        }
        if (cores && cores->count>0)
        {
             vVecCor[idx]   = Vector4f(float(vf3Cor[idx0].x)/255.0f, float(vf3Cor[idx0].y)/255.0f, float(vf3Cor[idx0].z)/255.0f, float(vf3Cor[idx0].w)/255.0f);
             vVecCor[idx+1] = Vector4f(float(vf3Cor[idx1].x)/255.0f, float(vf3Cor[idx1].y)/255.0f, float(vf3Cor[idx1].z)/255.0f, float(vf3Cor[idx1].w)/255.0f);
             vVecCor[idx+2] = Vector4f(float(vf3Cor[idx2].x)/255.0f, float(vf3Cor[idx2].y)/255.0f, float(vf3Cor[idx2].z)/255.0f, float(vf3Cor[idx2].w)/255.0f);
        }

       idx+=3;
    }
    
    Objeto3D ob;
    if(vVecCor.size()>0) //verifica se o arquivo de textura foi definido
        criarObjPosCor(ob, nome, vVecPos, vVecCor, vVecNorm);
    else if(vVecNorm.size()>0)
        criarObjPos(ob, nome, vVecPos, Material(), vVecNorm);
    else
        criarObjPos(ob, nome, vVecPos);

    objetos.push_back(move(ob));
    return true;
}



bool criarObjPosTex(Objeto3D &obj, string nome, vector<Vector3f> coordVertices, vector<Vector2f> coordTextura, string texturaKd, string texturaKs, vector<Vector3f> normais)
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
 
     if( coordTextura.size() != coordVertices.size())
     {
         ERRO(); //Erro, os arrays devem possuir o mesmo tamanho.";
         return false;
     }

     if(!normais.empty() && normais.size() != coordVertices.size())
    {
        ERRO(); //Erro, os arrays devem possuir o mesmo tamanho.";
        return false;
    }


    //se um objeto já foi criado, destroi ele primeiro
    excluirObjeto(obj);
    obj.nVertices = coordVertices.size();
    obj.nome = nome;
    obj.mat = obj.mat =  Material(MAT_PADRAO);
    obj.posicao = calcularCentroMediano(coordVertices);


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
    //-----------------


    //----------------- Textura
    glGenBuffers(1, &(obj.VBO_TEXTURA));
    glBindBuffer(GL_ARRAY_BUFFER, obj.VBO_TEXTURA);
    glBufferData(GL_ARRAY_BUFFER, coordTextura.size() * sizeof(Vector2f), &coordTextura.at(0), GL_STATIC_DRAW);
    glEnableVertexAttribArray(VERTEX_ATTRIB_TEX);  	//define como o buffer será acessado no vertex shader!
    glVertexAttribPointer(VERTEX_ATTRIB_TEX, 2,	GL_FLOAT, GL_FALSE,	0, (void*)0);
    //-----------------

    //----------------- Normal
    if(!normais.empty())
    {
        glGenBuffers(1, &(obj.VBO_NORMAL)); //cria um ponteiro para o buffer
        glBindBuffer(GL_ARRAY_BUFFER, obj.VBO_NORMAL); //bind 
        glBufferData(GL_ARRAY_BUFFER, normais.size() * sizeof(Vector3f), &normais.at(0), GL_STATIC_DRAW); // carra os dados para a GPU
        glEnableVertexAttribArray(VERTEX_ATTRIB_NORMAL);   
        glVertexAttribPointer(VERTEX_ATTRIB_NORMAL, 3,	GL_FLOAT, GL_FALSE,	0, (void*)0);
    }
    //-----------------

    //unbind all !!
    glBindVertexArray(0); 
    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    

    
    obj.texturaPtrKd = carregarTexturaOpenGL(texturaKd);
    if(fs::exists(texturaKs))
        obj.texturaPtrKs = carregarTexturaOpenGL(texturaKs);
    

    return true;
}


bool criarObjPosCor(Objeto3D &obj, string nome, vector<Vector3f> coordVertices, vector<Vector4f> corVertices, vector<Vector3f> normais)
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

    if(!normais.empty() && normais.size() != coordVertices.size())
    {
        ERRO(); //Erro, os arrays devem possuir o mesmo tamanho.";
        return false;
    }


    //se um objeto já foi criado, destroi ele primeiro
    excluirObjeto(obj);

    obj.nVertices = coordVertices.size();
    obj.nome = nome;
    obj.mat =  Material(MAT_PADRAO);
    obj.posicao = calcularCentroMediano(coordVertices);

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
    //-----------------


    //----------------- Cor
    glGenBuffers(1, &(obj.VBO_COR)); //cria um ponteiro para o buffer
    glBindBuffer(GL_ARRAY_BUFFER, obj.VBO_COR); //bind 
    glBufferData(GL_ARRAY_BUFFER, corVertices.size() * sizeof(Vector4f), &corVertices.at(0), GL_STATIC_DRAW); // carra os dados para a GPU
    glEnableVertexAttribArray(VERTEX_ATTRIB_COR);  
    glVertexAttribPointer(VERTEX_ATTRIB_COR, 4,	GL_FLOAT, GL_FALSE,	0, (void*)0);
    //-----------------


    //----------------- Normal
    if(!normais.empty())
    {
        glGenBuffers(1, &(obj.VBO_NORMAL)); //cria um ponteiro para o buffer
        glBindBuffer(GL_ARRAY_BUFFER, obj.VBO_NORMAL); //bind 
        glBufferData(GL_ARRAY_BUFFER, normais.size() * sizeof(Vector3f), &normais.at(0), GL_STATIC_DRAW); // carra os dados para a GPU
        glEnableVertexAttribArray(VERTEX_ATTRIB_NORMAL);   
        glVertexAttribPointer(VERTEX_ATTRIB_NORMAL, 3,	GL_FLOAT, GL_FALSE,	0, (void*)0);
    }
    //-----------------

    //unbind all !!
    glBindVertexArray(0); 
    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return true;

} //fim 


bool criarObjPos(Objeto3D &obj, string nome, vector<Vector3f> coordVertices, Material mat, vector<Vector3f> normais)
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

    if(!normais.empty() && normais.size() != coordVertices.size())
    {
        ERRO(); //Erro, os arrays devem possuir o mesmo tamanho.";
        return false;
    }


    //se um objeto já foi criado, destroi ele primeiro
    excluirObjeto(obj);

    obj.nVertices = coordVertices.size();
    obj.nome = nome;
    obj.mat = mat;
    obj.posicao = calcularCentroMediano(coordVertices);


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


    //----------------- Normal
    if(!normais.empty())
    {
        glGenBuffers(1, &(obj.VBO_NORMAL)); //cria um ponteiro para o buffer
        glBindBuffer(GL_ARRAY_BUFFER, obj.VBO_NORMAL); //bind 
        glBufferData(GL_ARRAY_BUFFER, normais.size() * sizeof(Vector3f), &normais.at(0), GL_STATIC_DRAW); // carra os dados para a GPU
        glEnableVertexAttribArray(VERTEX_ATTRIB_NORMAL);   
        glVertexAttribPointer(VERTEX_ATTRIB_NORMAL, 3,	GL_FLOAT, GL_FALSE,	0, (void*)0);
    }
    //-----------------

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

    if(obj.texturaPtrKd)
    {
        glActiveTexture(GL_TEXTURE0); //unidade de textura zero
        glBindTexture(GL_TEXTURE_2D, obj.texturaPtrKd);
    }

     if(obj.texturaPtrKs)
    {
        glActiveTexture(GL_TEXTURE1); //unidade de textura zero
        glBindTexture(GL_TEXTURE_2D, obj.texturaPtrKs);
    }

    //Ativa o VAO e os atributos dos vértices associados a este VAO
    glBindVertexArray(obj.VAO);

    //renderizar os triangulos
    glDrawArrays(GL_TRIANGLES, 0, (GLsizei) obj.nVertices);

    //desativa o VAO
    glBindVertexArray(0);


    if(obj.texturaPtrKd)
    {
        glBindTexture(GL_TEXTURE_2D, 0);
	    glActiveTexture(GL_TEXTURE0);
    }

    if(obj.texturaPtrKs)
    {
        glBindTexture(GL_TEXTURE_2D, 0);
	    glActiveTexture(GL_TEXTURE1);
    }

}

ShaderTipo definirShader(const Objeto3D& obj)
{
    if(obj.VBO_NORMAL) ///SHADER_MATERIAL_ILUM  | SHADER_TEX_ILUM 
    {
        if (obj.VBO_TEXTURA)
            return SHADER_TEX_ILUM;
        if (obj.VBO_COR)
            return SHADER_COR_VERT;
        else
            return SHADER_MATERIAL_ILUM;
    }
    else //SHADER_COR_SOLIDA | SHADER_COR_VERT | SHADER_TEX
    {
        if (obj.VBO_TEXTURA)
            return SHADER_TEX;
        if (obj.VBO_COR)
            return SHADER_COR_VERT;
        else
            return SHADER_COR_SOLIDA;
    }
    
    return SHADER_COR_SOLIDA; // padrão
}

Vector3f calcularCentroMediano(vector<Vector3f>& pontos) {
    Vector3f soma(0, 0, 0);
    
    // Soma todas as coordenadas dos vértices
    for (const auto& p : pontos) {
        soma += p;
    }

    // Divide pela quantidade de vértices para obter a média
    return soma / pontos.size();
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