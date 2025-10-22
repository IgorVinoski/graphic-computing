#include <glad/glad.h> //GLAD
#include <fstream>
#include <iostream>
#include "shader.hpp"
#include "erro.hpp"

GLuint criarPrograma(std::string arqVertexShader, std::string arqFragShader) 
{

	//variaveis para armazenar os ponteiros para: vextexshader, fragmentshader e programa
	GLuint vShader=0, fShader=0, program=0;

	//cria o vertex shader
	if (!compilarShader(GL_VERTEX_SHADER, arqVertexShader, &vShader)) {
		ERRO();
		return false;
	}
		

	//cria o fragment shader
	if (!compilarShader(GL_FRAGMENT_SHADER, arqFragShader, &fShader)) {
		ERRO();
		return false;
	}

	//cria o programa
	program = glCreateProgram();

	//anexar os shaders ao programa
	glAttachShader(program, vShader);
	glAttachShader(program, fShader);

	//passo final, link do programa
	glLinkProgram(program);

	//verifica erros no OpenGL
	if (verificarErros())
	{
		ERRO();
		return false;
	}
		

	//verifica erros no link do programa
	GLint linkValido;
	glGetProgramiv(program, GL_LINK_STATUS, &linkValido);
	if (!linkValido)
	{
		
		printProgramInfoLog(program);
		ERRO();
		return false;
	}

	//libera memoria dos shaders (eles já foram compilados e anexados ao programa)
	glDetachShader(program, vShader);
	glDetachShader(program, fShader);

	glDeleteShader(vShader);
	glDeleteShader(fShader);

	//retorna o ponteiro do programa
	return program;

}


bool excluirPrograma(GLuint ID){
	if(ID){
		glDeleteProgram( ID );
		return true;
	}
	else
		return false;
		
}

bool compilarShader(int type, std::string path, unsigned int* ID) {


	//le o arquivo contendo o codigo fonte do shader
	std::ifstream f(path);

	if (!f.is_open()) {
		ERRO();
		return false;
	}
	std::string strFile((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
	const char* strCode = strFile.c_str();

	//cria o shader
	GLuint shaderRef = glCreateShader(type);

	//especifica o codigo fonte do shader
	glShaderSource(shaderRef, 1, &strCode, NULL);

	//compila o shader
	glCompileShader(shaderRef);

	//verifica erros no OpenGL
	if (verificarErros())
	{
		ERRO();
		return false;
	}

	//verifica erros na compilação do shader
	GLint compilou;
	glGetShaderiv(shaderRef, GL_COMPILE_STATUS, &compilou);
	if (!compilou)
	{
		std::cout << "Errou ao compilar o shader do tipo " << type << std::endl;
		printShaderInfoLog(shaderRef);
		ERRO();
		return false;

	}

	//retorna a referencia do shader
	*ID = shaderRef;
	return true;
}





bool verificarErros() {
	bool er = false;
	int erros = glGetError();
	while (erros != GL_NO_ERROR) {
		std::cout << "glError: " << erros << std::endl;
		erros = glGetError();

		er = true;
	}

	return er;

}

void printProgramInfoLog(unsigned int progama) {
	int len = 0;
	int chWrittn = 0;
	char* log;
	glGetProgramiv(progama, GL_INFO_LOG_LENGTH, &len);
	if (len > 0) {
		log = (char*)malloc(len);
		glGetProgramInfoLog(progama, len, &chWrittn, log);
		
		std::cout << "ProgramInfoLog: " << log << std::endl;
		free(log);
	}
}


void printShaderInfoLog(unsigned int shaderRef) {
	int len = 0;
	int chWrittn = 0;
	char* log;
	glGetShaderiv(shaderRef, GL_INFO_LOG_LENGTH, &len);
	if (len > 0) {
		log = (char*)malloc(len);
		glGetShaderInfoLog(shaderRef, len, &chWrittn, log);
		
		std::cout << "ShaderInfoLog: " << log << std::endl;

		free(log);
	}
}




