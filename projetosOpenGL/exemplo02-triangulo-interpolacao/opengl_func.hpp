#ifndef OPENGL_FUNC_H_INCLUDED
#define OPENGL_FUNC_H_INCLUDED


// GLSL 3.3 -> MAJOR.MINOR
#define  GLSL_MAJOR_VERSION 3
#define  GLSL_MINOR_VERSION 3

#include <iostream>

#include <GLFW/glfw3.h> //GLFW



//cria a janela para a rendeização, cria-se o contexto do OpenGL.
GLFWwindow *criarJanela(int larg, int alt, string titulo, bool tela_cheia = false);

//destruir a janela
void destruirJanela(GLFWwindow *jan);

// imprime informacoes sobre driver e versão do OpenGL. Só execute esta função após definir um contexto/janela do OpenGL
void imprimirInfOpenGL(); 


#endif