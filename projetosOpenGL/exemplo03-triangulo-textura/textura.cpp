#include <iostream>
#include <stb/stb_image.h>

#include <glad/glad.h> //GLAD
#include <GLFW/glfw3.h> //GLFW

#include "textura.hpp"

using namespace std;

GLuint carregarTexturaOpenGL(std::string arquivo, bool verticalFlip, bool mipmap, bool filtroAnisotropico)
{
    int largura; 
    int altura;
    int canais;

    
    if(verticalFlip)
        stbi_set_flip_vertically_on_load(true); 


    unsigned char *dadosImg = stbi_load(arquivo.c_str(), &largura, &altura, &canais, 0); 
	if (!dadosImg){
        cout << "carregarTexturaOpenGL: Erro ao ler o arquivo " << arquivo << endl;
        return false;
    }

    //cout << path << " - " << largura << " - " << altura << " - " << canais << "\n";
    
    GLenum formato;
    if (canais == 1)
        formato = GL_RED;
    else if (canais == 3)
        formato = GL_RGB;
    else if (canais == 4)
        formato = GL_RGBA;

    //gera um ponteiro para a texura e carrega a imagem na GPU
    GLuint texturaPtr = 0;
    glGenTextures(1, &texturaPtr);
    glBindTexture(GL_TEXTURE_2D, texturaPtr);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //use isso, pois em imagens grandes (2253x2352) o opengl se perde na leitura da imagem
    glTexImage2D(GL_TEXTURE_2D, 0, formato, largura, altura, 0, formato, GL_UNSIGNED_BYTE, dadosImg);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


    //Ativar mipmap?
    if(mipmap){
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST); //default
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//default
    }
    else{
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        
    }


    if (filtroAnisotropico && glfwExtensionSupported("GL_EXT_texture_filter_anisotropic")) {
        GLfloat anisoset = 0.0f;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &anisoset);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, anisoset);
    }

  

    //libera memoria
	glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free( dadosImg );
	return texturaPtr;
}