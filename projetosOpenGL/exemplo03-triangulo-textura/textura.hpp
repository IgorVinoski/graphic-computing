#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED

#include <string>

GLuint  carregarTexturaOpenGL(std::string arquivo, bool verticalFlip=true, bool mipmap=true, bool filtroAnisotropico =true);

#endif