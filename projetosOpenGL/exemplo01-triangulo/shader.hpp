#ifndef SHADER_H_INCLUDED
#define SHADER_H_INCLUDED

#include <string>




GLuint criarPrograma(std::string arqVertexShader, std::string arqFragShader);

bool excluirPrograma(GLuint ID);

bool compilarShader(int type, std::string path, unsigned int* ID);



bool verificarErros();

void printProgramInfoLog(unsigned int progama);

void printShaderInfoLog(unsigned int shaderRef);


#endif
