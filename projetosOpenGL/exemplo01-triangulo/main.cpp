#include <iostream>
#include <string> 
#include <vector>
#include <Eigen/Dense>

#include <filesystem> // C++17
namespace fs = std::filesystem;

#include <glad/glad.h> //GLAD
#include <GLFW/glfw3.h> //GLFW


using namespace std;
using namespace Eigen;


#include "opengl_func.hpp"
#include "shader.hpp"
#include "CGMath.hpp"
#include "objeto3d.hpp"
#include "erro.hpp"


int main(void) 
{

	//O diretório de trabalho é a pasta onde estão os arquivos do projeto
	if (fs::current_path().filename() == "build") 
		fs::current_path(fs::current_path().parent_path());

	//--------JANELA--------
	GLFWwindow * janela = criarJanela(1024, 768, "Exemplo 01 - Triangulo", true); //inicializa as bibliotecas GLAD e GLFW. Cria-se o contexto do opengl e janela


	//--------Versao OpenGL--------
	imprimirInfOpenGL();
	
	//--------SHADERS--------
	 //compila os shaders, gerando um programa
	GLuint programaShader = criarPrograma ("shaders/vertShader.glsl", "shaders/fragShader.glsl");
	if(!programaShader)
	{
		glfwDestroyWindow(janela);
		glfwTerminate();
        ERRO(); //termina a execução
	}



	//--------Projecao--------
	float vFOV=60.0f, zNear=0.1f, zFar=200.0f;
	int largJan, altJan;
	glfwGetWindowSize(janela, &largJan, &altJan);
	Matrix4f matProj = getMatProj(largJan, altJan, vFOV, zNear, zFar);

	//--------Modelos 3D--------
	Objeto3D obj;
	obj.cor = Vector4f(1.0f, 0.5f, 0.5f, 1.0f);
	vector<Vector3f> coordVertices;
	coordVertices.push_back(Vector3f(-0.5f, -0.5f, -3.0f));
	coordVertices.push_back(Vector3f(0.5f, -0.5f, -3.0f));
	coordVertices.push_back(Vector3f(0.0f, 0.5f, -3.0f));
	criarObjPos(obj, "triangulo", coordVertices);
	


	//--------CONFIGURACOES DO OPENGL--------
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //cor de fundo da tela - > RGBA

	//ativar o teste de profundidade para cada fragmento processado dentro do fragment shader
	//antes de cada fragmento ser desenhado é realizada uma comparação com o valor de profundidade
	glEnable(GL_DEPTH_TEST);

	//define qual algoritmo sera usado para comparar os valores de profundidade dentro do fragment shader
	//https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glDepthFunc.xhtml
	glDepthFunc(GL_LESS);


	//--------LOOP DE RENDERIZACAO--------
	while (!glfwWindowShouldClose(janela) && glfwGetKey(janela, GLFW_KEY_ESCAPE) != GLFW_PRESS) //a cada iteração, um quadro é renderizado!
	{

		
		//Limpa os buffers que armazenam os valores de pronfundidade de cada fragmento para seu valor inicial (1.0)
		//Caso isso não seja feito, os fragmentos de novos triângulos serão comparados com os fragmentos renderizados do quadro anterior!
		glClear(GL_DEPTH_BUFFER_BIT);

		//Limpa os buffers que armazenam os valores de cor
		glClear(GL_COLOR_BUFFER_BIT);


		//usa os shaders previamente compilados
		glUseProgram(programaShader);

		//passamos a cor para o fragment shader
		GLuint uniformRef = glGetUniformLocation(programaShader, "corFrag");
		glUniform3fv(uniformRef, 1, obj.cor.data());

		//passamos a matriz para o vertexshader
		uniformRef = glGetUniformLocation(programaShader, "mat");
		glUniformMatrix4fv(uniformRef, 1, GL_FALSE, matProj.data());

		//renderiza o objeto
		renderizarObjeto(obj);
		
		
		glfwPollEvents(); //processa os eventos de teclado e mouse da janela

		//atualiza a janela de visualização com o quadro renderizado
		glfwSwapBuffers(janela);
		
		
	} //fim while renderizacao
	
	//--------DESALOCA MEMORIA--------
	//remove os shaders
	glDeleteProgram(programaShader);

	excluirObjeto(obj);
	
	destruirJanela(janela);


	return EXIT_SUCCESS;

} //fim main

