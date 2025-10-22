#include <iostream>
#include <string> 
#include <vector>
#include <map>
#include <chrono>
#include <Eigen/Dense>

#include <glad/glad.h> //GLAD
#include <GLFW/glfw3.h> //GLFW


using namespace std;
using namespace Eigen;
using namespace chrono;


#include "opengl_func.hpp"
#include "shader.hpp"
#include "CGMath.hpp"
#include "objeto3d.hpp"
#include "erro.hpp"



void keyEvent(GLFWwindow *win, int key, int scancode, int action, int mods);

#include <filesystem> // C++17
namespace fs = std::filesystem;


bool wireframe = false;
bool animacao = false;
float transLX = 0.0;


int main(void) 
{

	//O diretório de trabalho é a pasta onde estão os arquivos do projeto
	if (fs::current_path().filename() == "build") 
		fs::current_path(fs::current_path().parent_path()); 

	
	//--------JANELA--------
	GLFWwindow * janela = criarJanela(1024, 768, "Exemplo 04", false); //inicializa as bibliotecas GLAD e GLFW. Cria-se o contexto do opengl e janela
	
	

	//--------Versao OpenGL--------
	imprimirInfOpenGL();
	
	//--------SHADERS--------
	//compila os shaders
	map<ShaderTipo, GLuint> shaders; //array de shaders compilados
	shaders[SHADER_COR_SOLIDA] = criarPrograma ("shaders/corSolidaVtx.glsl", "shaders/corSolidaFrag.glsl");
	shaders[SHADER_COR_VERT] = criarPrograma ("shaders/corVerticeVtx.glsl", "shaders/corVerticeFrag.glsl");
	shaders[SHADER_TEX] = criarPrograma ("shaders/texturaVtx.glsl", "shaders/texturaFrag.glsl");

	for (const auto& [tipo, prog] : shaders)
	{
		if(!prog) //verifica se foi compilado com sucesso
		{
			glfwDestroyWindow(janela);
			glfwTerminate();
			ERRO(); //termina a execução
		}
	}


	//--------Projecao--------
	float vFOV=60.0f, zNear=0.1f, zFar=200.0f;
	int largJan, altJan;
	glfwGetWindowSize(janela, &largJan, &altJan);
	Matrix4f matProj = getMatProj(largJan, altJan, vFOV, zNear, zFar);
	Matrix4f matLookAt = getMatLookAt(Vector3f(0,5,15), Vector3f(0,0, 0));
	Matrix4f matPV = matProj * matLookAt;

	//--------Modelos 3D--------
	
	//vector<Objeto3D> ob = carregarOBJ("../OBJs/chao.obj");
	vector<Objeto3D> objetos;
	carregarOBJ(objetos, "../Objetos3D/chao.obj", "chao");
	carregarOBJ(objetos, "../Objetos3D/tambor-azul.obj", "tambor");
	carregarPLY(objetos, "../Objetos3D/cubo-corvert.ply", "cubo");

	int somaTriangulos = 0;
	for (const auto& obj : objetos)
	{
		cout << "Obj[" << obj.nome << "] posicao: " << obj.posicao.transpose() << endl;
		cout << "Obj[" << obj.nome << "] triangulos: " << obj.nVertices / 3 << endl << endl;
		somaTriangulos += obj.nVertices / 3;
	}
	cout << "Total de triangulos na cena: " << somaTriangulos << endl << endl;


	//--------EVENTOS--------
	glfwSetKeyCallback(janela, keyEvent); //registra o evento de teclado para função keyEvent


	//--------CONFIGURACOES DO OPENGL--------
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //cor de fundo da tela - > RGBA

	//ativar o teste de profundidade para cada fragmento processado dentro do fragment shader
	//antes de cada fragmento ser desenhado é realizada uma comparação com o valor de profundidade
	glEnable(GL_DEPTH_TEST);

	//define qual algoritmo sera usado para comparar os valores de profundidade dentro do fragment shader
	//https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glDepthFunc.xhtml
	glDepthFunc(GL_LESS);

	GLuint uniformRef;

	time_point tempoRegistrado = high_resolution_clock::now();
	int contQuadros = 0;
	float angAnima = 0.0;

	//--------LOOP DE RENDERIZACAO--------
	while (!glfwWindowShouldClose(janela) && glfwGetKey(janela, GLFW_KEY_ESCAPE) != GLFW_PRESS) //a cada iteração, um quadro é renderizado!
	{

		
		//Limpa os buffers que armazenam os valores de pronfundidade de cada fragmento para seu valor inicial (1.0)
		//Caso isso não seja feito, os fragmentos de novos triângulos serão comparados com os fragmentos renderizados do quadro anterior!
		glClear(GL_DEPTH_BUFFER_BIT);

		//Limpa os buffers que armazenam os valores de cor
		glClear(GL_COLOR_BUFFER_BIT);



		//wireframe		
		if(wireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


		//renderiza o objeto
		for (size_t i = 0; i < objetos.size(); ++i) {
			Objeto3D& obj = objetos[i];

			ShaderTipo tipo = definirShader(obj); //escolhe o shader com base no objeto a ser renderizado
			GLuint programa = shaders[tipo];
    		glUseProgram(programa);

			if(obj.nome=="tambor" && animacao== true){
				angAnima += 0.0005f;
				Matrix4f M = getMatTranslacao(obj.posicao) *
							 getMatRotacaoY(angAnima) * 
							 getMatTranslacao(-obj.posicao);
				M = matPV * M; 
				//passamos a matriz para o vertexshader
				uniformRef = glGetUniformLocation(programa, "mat");
				glUniformMatrix4fv(uniformRef, 1, GL_FALSE, M.data());
			}
			else if(obj.nome=="cubo"){
				
				Matrix4f M2 = getMatTranslacao(obj.posicao) *
							 getMatTranslacao(Vector3f(0,0,transLX)) * 
							 getMatTranslacao(-obj.posicao);
				M2 = matPV * M2; 
				//passamos a matriz para o vertexshader
				uniformRef = glGetUniformLocation(programa, "mat");
				glUniformMatrix4fv(uniformRef, 1, GL_FALSE, M2.data());
			}else{
				//passamos a matriz para o vertexshader
				uniformRef = glGetUniformLocation(programa, "mat");
				glUniformMatrix4fv(uniformRef, 1, GL_FALSE, matPV.data());
			}

			
			

			if(tipo == SHADER_COR_SOLIDA)
			{
				uniformRef = glGetUniformLocation(programa, "cor");
				glUniform4fv(uniformRef, 1, obj.cor.data());
			}
			

			renderizarObjeto(obj);
		}
		

		//evento teclado dentro dentro do loop de renderização
		if(glfwGetKey(janela, GLFW_KEY_W) == GLFW_PRESS)
		{
			cout << "Teste " << transLX << endl;
			transLX += 0.01f;
		}
		else if(glfwGetKey(janela, GLFW_KEY_S) == GLFW_PRESS)
		{
			cout << "Teste " << transLX << endl;
			transLX -= 0.01f;
		}
	
		
		
		glfwPollEvents(); //processa os eventos de teclado e mouse da janela

		//atualiza a janela de visualização com o quadro renderizado
		glfwSwapBuffers(janela);

		
		//FPS-------------
		contQuadros++;
		time_point tempoAgora = high_resolution_clock::now();

		//duration = representa um intervalo de temp, a diferença entre dois pontos no tempo.
		duration<float> duracao = tempoAgora - tempoRegistrado;

        if (duracao.count() >= 1.0f) //já passou 1.0 segundos?
		{
			//imprime o FPS no título da janela
			string strFPS = "FPS: " + to_string(contQuadros);
			cout << strFPS << endl;
            glfwSetWindowTitle(janela, strFPS.c_str()); 


			contQuadros = 0;
            tempoRegistrado = tempoAgora;
        }
		//---------------
		
		
	} //fim while renderizacao
	
	//--------DESALOCA MEMORIA--------
	//remove os shaders
	for (auto& [tipo, prog] : shaders)
		glDeleteProgram(prog);

	//excluirObjeto(&obj);
	for (auto& ob: objetos)
		excluirObjeto(ob);
	objetos.clear();

	destruirJanela(janela);


	return EXIT_SUCCESS;

} //fim main


void keyEvent(GLFWwindow *win, int key, int scancode, int action, int mods)
{

	if (key == GLFW_KEY_F5 && action == GLFW_PRESS)
	{
		wireframe = !wireframe;
	}

	if (key == GLFW_KEY_F6 && action == GLFW_PRESS)
	{
		animacao = !animacao;
	}

	


}


